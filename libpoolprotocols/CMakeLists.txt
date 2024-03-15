cmake_minimum_required(VERSION 3.24)
project(poolprotocols)

set(SOURCES
	PoolURI.cpp PoolURI.h
	PoolClient.h
	PoolManager.h PoolManager.cpp
	testing/SimulateClient.h testing/SimulateClient.cpp
	stratum/EthStratumClient.h stratum/EthStratumClient.cpp
	getwork/EthGetworkClient.h getwork/EthGetworkClient.cpp
)

add_library(poolprotocols ${SOURCES})
target_link_libraries(poolprotocols PRIVATE devcore ethminer-buildinfo ethash::ethash  ${Boost_LIBRARY_DIR_RELEASE} jsoncpp_static ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
include_directories(AFTER .. ${OPENSSL_INCLUDE_DIR})
