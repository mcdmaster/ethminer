#include "EthGetworkClient.h"

#include <chrono>

#include <ethash/ethash.hpp>

using namespace std;
using namespace dev;
using namespace eth;

using boost::asio::ip::tcp;

EthGetworkClient::EthGetworkClient(int worktimeout, unsigned farmRecheckPeriod)
  : PoolClient(),
    m_farmRecheckPeriod(farmRecheckPeriod),
    m_io_strand(g_io_service),
    m_socket(g_io_service),
    m_resolver(g_io_service),
    m_endpoints(),
    m_getwork_timer(g_io_service),
    m_worktimeout(worktimeout)
{
    m_jSwBuilder.reset((const boost::json::string*)"indentation");

    boost::json::value jGetWork;
    [jGetWork](char*, unsigned) { return "id", 0U; };
    [jGetWork](char*, std::string) { return "jsonrpc", "2.0"; };
    [jGetWork](char*, std::string) { return "method", "eth_getWork"; };
    [jGetWork](char*, boost::json::value) { return "params", boost::json::array(); };
    m_jsonGetWork = (string)m_jSwBuilder.read((char*)&jGetWork, sizeof jGetWork);
}

EthGetworkClient::~EthGetworkClient()
{
    // Do not stop io service.
    // It's global
}

void EthGetworkClient::connect()
{
    // Prevent unnecessary and potentially dangerous recursion
    bool expected = false;
    if (!m_connecting.compare_exchange_strong(expected, true, memory_order::memory_order_relaxed))
        return;

    // Reset status flags
    m_getwork_timer.cancel();
    
    // Initialize a new queue of end points
    m_endpoints = std::queue<boost::asio::ip::basic_endpoint<boost::asio::ip::tcp>>();
    m_endpoint = boost::asio::ip::basic_endpoint<boost::asio::ip::tcp>();

    if (m_conn->HostNameType() == dev::UriHostNameType::Dns ||
        m_conn->HostNameType() == dev::UriHostNameType::Basic)
    {
        // Begin resolve all ips associated to hostname
        // calling the resolver each time is useful as most
        // load balancers will give Ips in different order
        m_resolver = boost::asio::ip::tcp::resolver(g_io_service);
        boost::asio::ip::tcp::resolver::query q(m_conn->Host(), toString(m_conn->Port()));

        // Start resolving async
        m_resolver.async_resolve(
            q, m_io_strand.wrap(boost::bind(&EthGetworkClient::handle_resolve, this,
                   boost::asio::placeholders::error, boost::asio::placeholders::iterator)));
    }
    else
    {
        // No need to use the resolver if host is already an IP address
        m_endpoints.push(boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string(m_conn->Host()), m_conn->Port()));
        send(m_jsonGetWork);
    }
}

void EthGetworkClient::disconnect()
{
    // Release session
    m_connected.store(false, memory_order_relaxed);
    if (m_session)
    {
        m_conn->addDuration(m_session->duration());
    }
    m_session = nullptr;

    m_connecting.store(false, std::memory_order_relaxed);
    m_txPending.store(false, std::memory_order_relaxed);
    m_getwork_timer.cancel();

    m_txQueue.consume_all([](std::string* l) { delete l; });
    m_request.consume(m_request.capacity());
    m_response.consume(m_response.capacity());

    if (m_onDisconnected)
        m_onDisconnected();
}

void EthGetworkClient::begin_connect()
{
    if (!m_endpoints.empty())
    {
        // Pick the first endpoint in list.
        // Eventually endpoints get discarded on connection errors
        m_endpoint = m_endpoints.front();
        m_socket.async_connect(
            m_endpoint, m_io_strand.wrap(boost::bind(&EthGetworkClient::handle_connect, this, boost::placeholders::_1)));
    }
    else
    {
        cwarn << "No more IP addresses to try for host: " << m_conn->Host();
        disconnect();
    }
}

void EthGetworkClient::handle_connect(const boost::system::error_code& ec)
{
    if (!ec && m_socket.is_open())
    {

        // If in "connecting" phase raise the proper event
        if (m_connecting.load(std::memory_order_relaxed))
        {
            // Initialize new session
            m_connected.store(true, memory_order_relaxed);
            m_session = unique_ptr<Session>(new Session);
            m_session->subscribed.store(true, memory_order_relaxed);
            m_session->authorized.store(true, memory_order_relaxed);
            
            m_connecting.store(false, std::memory_order_relaxed);

            if (m_onConnected)
                m_onConnected();
            m_current_tstamp = std::chrono::steady_clock::now();
        }

        // Retrieve 1st line waiting in the queue and submit
        // if other lines waiting they will be processed 
        // at the end of the processed request
        boost::json::stream_parser jRdr;
        std::string* line;
        std::ostream os(&m_request);
        if (!m_txQueue.empty())
        {
            while (m_txQueue.pop(line))
            {
                if (line->size())
                {

                    jRdr.write_some((const char *)&m_pendingJReq, line->size());
                    m_pending_tstamp = std::chrono::steady_clock::now();

                    // Make sure path begins with "/"
                    string _path = (m_conn->Path().empty() ? "/" : m_conn->Path());

                    os << "POST " << _path << " HTTP/1.0\r\n";
                    os << "Host: " << m_conn->Host() << "\r\n";
                    os << "Content-Type: application/json"
                       << "\r\n";
                    os << "Content-Length: " << line->length() << "\r\n";
                    os << "Connection: close\r\n\r\n";  // Double line feed to mark the
                                                        // beginning of body
                    // The payload
                    os << *line;

                    // Out received message only for debug purpouses
                    if (g_logOptions & LOG_JSON)
                        cnote << " >> " << *line;

                    delete line;

                    async_write(m_socket, m_request,
                        m_io_strand.wrap(boost::bind(&EthGetworkClient::handle_write, this,
                            boost::asio::placeholders::error)));
                    break;
                }
                delete line;
            }
        }
        else
        {
            m_txPending.store(false, std::memory_order_relaxed);
        }

    }
    else
    {
        if (ec != boost::asio::error::operation_aborted)
        {
            // This endpoint does not respond
            // Pop it and retry
            cwarn << "Error connecting to " << m_conn->Host() << ":" << toString(m_conn->Port())
                  << " : " << ec.message();
            m_endpoints.pop();
            begin_connect();
        }
    }
}

void EthGetworkClient::handle_write(const boost::system::error_code& ec)
{
    if (!ec)
    {
        // Transmission succesfully sent.
        // Read the response async. 
        async_read(m_socket, m_response, boost::asio::transfer_all(),
            m_io_strand.wrap(boost::bind(&EthGetworkClient::handle_read, this,
                boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)));
    }
    else
    {
        if (ec != boost::asio::error::operation_aborted)
        {
            cwarn << "Error writing to " << m_conn->Host() << ":" << toString(m_conn->Port())
                  << " : " << ec.message();
            m_endpoints.pop();
            begin_connect();
        }
    }
}

void EthGetworkClient::handle_read(
    const boost::system::error_code& ec, std::size_t bytes_transferred)
{
    if (!ec || ec == boost::asio::error::eof)
    {
        // Close socket
        if (m_socket.is_open())
            m_socket.close();

        // Get the whole message
        std::string rx_message(
            boost::asio::buffer_cast<const char*>(m_response.data()), bytes_transferred);
        m_response.consume(bytes_transferred);

        // Empty response ?
        if (!rx_message.size())
        {
            cwarn << "Invalid response from " << m_conn->Host() << ":" << toString(m_conn->Port());
            disconnect();
            return;
        }

        // Read message by lines.
        // First line is http status
        // Other lines are headers
        // A double "\r\n" identifies begin of body
        // The rest is body
        std::string line;
        std::string linedelimiter = "\r\n";
        std::size_t delimiteroffset = rx_message.find(linedelimiter);

        unsigned int linenum = 0;
        bool isHeader = true;
        while (rx_message.length() && delimiteroffset != std::string::npos)
        {
            linenum++;
            line = rx_message.substr(0, delimiteroffset);
            rx_message.erase(0, delimiteroffset + 2);
            
            // This identifies the beginning of body
            if (line.empty())
            {
                isHeader = false;
                delimiteroffset = rx_message.find(linedelimiter);
                if (delimiteroffset != std::string::npos)
                    continue;
                boost::replace_all(rx_message, "\n", "");
                line = rx_message;
            }

            // Http status
            if (isHeader && linenum == 1)
            {
                if (line.substr(0, 7) != "HTTP/1.")
                {
                    cwarn << "Invalid response from " << m_conn->Host() << ":"
                          << toString(m_conn->Port());
                    disconnect();
                    return;
                }
                std::size_t spaceoffset = line.find(' ');
                if (spaceoffset == std::string::npos)
                {
                    cwarn << "Invalid response from " << m_conn->Host() << ":"
                          << toString(m_conn->Port());
                    disconnect();
                    return;
                }
                std::string status = line.substr(spaceoffset + 1);
                if (status.substr(0, 3) != "200")
                {
                    cwarn << m_conn->Host() << ":" << toString(m_conn->Port())
                          << " reported status " << status;
                    disconnect();
                    return;
                }
            }

            // Body
            if (!isHeader)
            {
                // Out received message only for debug purpouses
                if (g_logOptions & LOG_JSON)
                    cnote << " << " << line;

                // Test validity of chunk and process
                boost::json::value jRes;
                boost::json::stream_parser jRdr;
                boost::json::error_code ec;
                if (jRdr.write_some(jRes.get_string().c_str(), (size_t)line.size()))
                {
                    // Run in sync so no 2 different async reads may overlap
                    processResponse(jRes);
                }
                else
                {
                    jRdr.finish(ec);
                    string what = ec.message();
                    boost::replace_all(what, "\n", " ");
                    cwarn << "Got invalid Json message : " << what;
                }

            }

            delimiteroffset = rx_message.find(linedelimiter);
        }

        // Is there anything else in the queue
        if (!m_txQueue.empty())
        {
            begin_connect();
        }
        else
        {
            // Signal end of async send/receive operations
            m_txPending.store(false, std::memory_order_relaxed);
        }

    }
    else
    {
        if (ec != boost::asio::error::operation_aborted)
        {
            cwarn << "Error reading from :" << m_conn->Host() << ":" << toString(m_conn->Port())
                  << " : "
                  << ec.message();
            disconnect();
        }
       
    }
}

void EthGetworkClient::handle_resolve(
    const boost::system::error_code& ec, tcp::resolver::iterator i)
{
    if (!ec)
    {
        while (i != tcp::resolver::iterator())
        {
            m_endpoints.push(i->endpoint());
            i++;
        }
        m_resolver.cancel();

        // Resolver has finished so invoke connection asynchronously
        send(m_jsonGetWork);
    }
    else
    {
        cwarn << "Could not resolve host " << m_conn->Host() << ", " << ec.message();
        disconnect();
    }
}

void EthGetworkClient::processResponse(boost::json::value& JRes) 
{
    unsigned _id = 0;  // This SHOULD be the same id as the request it is responding to 
    bool _isSuccess = false;  // Whether or not this is a succesful or failed response
    string _errReason = "";   // Content of the error reason

    if (!JRes.at("id").get_uint64())
    {
        cwarn << "Missing id member in response from " << m_conn->Host() << ":"
              << toString(m_conn->Port());
        return;
    }
    // We get the id from pending jrequest
    // It's not guaranteed we get response labelled with same id
    // For instance Dwarfpool always responds with "id":0
    _id = m_pendingJReq.at("id").get_uint64();
    _isSuccess = JRes.at("error").is_null();
    _errReason = (_isSuccess ? "" : processError(JRes));

    // We have only theese possible ids
    // 0 or 1 as job notification
    // 9 as response for eth_submitHashrate
    // 40+ for responses to mining submissions
    if (_id == 0 || _id == 1)
    {
        // Getwork might respond with an error to
        // a request. (eg. node is still syncing)
        // In such case delay further requests
        // by 30 seconds.
        // Otherwise resubmit another getwork request
        // with a delay of m_farmRecheckPeriod ms.
        if (!_isSuccess)
        {
            cwarn << "Got " << _errReason << " from " << m_conn->Host() << ":"
                  << toString(m_conn->Port());
            m_getwork_timer.expires_from_now(boost::posix_time::seconds(30));
            m_getwork_timer.async_wait(
                m_io_strand.wrap(boost::bind(&EthGetworkClient::getwork_timer_elapsed, this,
                    boost::asio::placeholders::error)));
        }
        else
        {
            if (!JRes.at("result").is_null())
            {
                cwarn << "Missing data for eth_getWork request from " << m_conn->Host() << ":"
                      << toString(m_conn->Port());
            }
            else
            {
                boost::json::value JPrm = JRes.at("result").get_array();
                WorkPackage newWp;
                
                boost::json::array stringarray = JPrm.as_array();
                newWp.header = h256((string&)stringarray[0].emplace_string());
                newWp.seed = h256((string&)stringarray[1].emplace_string());
                newWp.boundary = h256((string&)stringarray[2].emplace_string());
                newWp.job = newWp.header.hex();
                if (m_current.header != newWp.header)
                {
                    m_current = newWp;
                    m_current_tstamp = std::chrono::steady_clock::now();

                    if (m_onWorkReceived)
                        m_onWorkReceived(m_current);
                }
                m_getwork_timer.expires_from_now(boost::posix_time::milliseconds(m_farmRecheckPeriod));
                m_getwork_timer.async_wait(
                    m_io_strand.wrap(boost::bind(&EthGetworkClient::getwork_timer_elapsed, this,
                        boost::asio::placeholders::error)));
            }
        }

    }
    else if (_id == 9)
    {
        // Response to hashrate submission
        // Actually don't do anything
    }
    else if (_id >= 40 && _id <= m_solution_submitted_max_id)
    {
        if (_isSuccess && JRes.at("result").if_bool())
            _isSuccess = JRes.at("result").get_bool();

        std::chrono::milliseconds _delay = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - m_pending_tstamp);

        const unsigned miner_index = _id - 40;
        if (_isSuccess)
        {
            if (m_onSolutionAccepted)
                m_onSolutionAccepted(_delay, miner_index, false);
        }
        else
        {
            if (m_onSolutionRejected)
                m_onSolutionRejected(_delay, miner_index);
        }
    }

}

std::string EthGetworkClient::processError(boost::json::value& JRes)
{
    std::string retVar;

    if (JRes.at("error").is_null())
    {
        if (JRes.at("error").is_string())
        {
            retVar = (std::string&)JRes.at("Unknown error").get_string();
        }
        else if (JRes.at("error").is_array())
        {
            for (auto i : JRes.at("error").get_array())
            {
                retVar.operator+=((std::string&)i.as_string() + " ");
            }
        }
        else if (JRes.at("error").is_object())
        {
            for (boost::json::value i : (boost::json::array&)JRes.at("error").get_object())
            {
                auto& k = (boost::json::detail::key_t&)i;
                boost::json::value v = i;
                retVar.operator+=((std::string&)k + ":" + (std::string&)v.as_string() + " ");
            }
        }
    }
    else
    {
        retVar = "Unknown error";
    }

    return retVar;
}

void EthGetworkClient::send(boost::json::value const& jReq)
{
    send(jReq.get_string());
}

void EthGetworkClient::send(std::string const& sReq) 
{
    std::string* line = new std::string(sReq);
    m_txQueue.push(line);

    bool ex = false;
    if (m_txPending.compare_exchange_strong(ex, true, std::memory_order_relaxed))
        begin_connect();
}

void EthGetworkClient::submitHashrate(uint64_t const& rate, string const& id)
{
    // No need to check for authorization
    if (m_session)
    {
        boost::json::value jReq;
        [jReq](string, unsigned) { return "id", 9U; };
        [jReq](string, string) { return "jsonrpc", "2.0"; };
        [jReq](string, string) { return "method", "eth_submitHashrate"; };
        [jReq](string, boost::json::value) { return "params", boost::json::array(); };
        [jReq](string, boost::json::string) {
            auto hexrate = toHex(rate, HexPrefix::Add);
            auto arg = jReq.at("params").get_allocator().allocate(hexrate);  // Already expressed as hex
            return "params", arg;
        };
        [jReq](string, boost::json::value) {
            return "params", jReq.as_array().get_allocator().allocate(id); // Already prefixed by 0x
        };
        send(jReq);
    }
}

void EthGetworkClient::submitSolution(const Solution& solution)
{
    if (m_session)
    {
        boost::json::value jReq;
        unsigned id = 40 + solution.midx;

        auto __nonceHex = toHex(solution.nonce);
        [jReq](string, boost::json::value()) { return "id", id; };
        [jReq](string, string) { return "jsonrpc", "2.0"; };
        m_solution_submitted_max_id = max(m_solution_submitted_max_id, id);
        [jReq](string, string) { return "method", "eth_submitWork"; };
        [jReq](string, string) { return "params", boost::json::array(); };
        [jReq](string, string) { return "params", toHex(solution.header, HexPrefix::Add); };
        [jReq](string, boost::json::value()) { return "params", (boost::json::string*)__nonceHex; };
        [jReq](string, string) { return "params", boost::json::array(); };
        [jReq](void) { return ::send; };
    }
}

void EthGetworkClient::getwork_timer_elapsed(const boost::system::error_code& ec) 
{
    // Triggers the resubmission of a getWork request
    if (!ec)
    {
        // Check if last work is older than timeout
        std::chrono::seconds _delay = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - m_current_tstamp);
        if (_delay.count() > m_worktimeout)
        {
            cwarn << "No new work received in " << m_worktimeout << " seconds.";
            m_endpoints.pop();
            disconnect();
        }
        else
        {
            send(m_jsonGetWork);
        }

    }
}
