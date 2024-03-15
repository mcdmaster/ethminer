<<<<<<< Updated upstream
cmake_minimum_required(VERSION 3.24)
project(poolprotocols)
=======
set(CMAK_C_COMPILER C:/Progra~1/NVIDIA~2/CUDA/v11.7/bin/nvcc.exe)
set(CMAK_CXX_COMPILER C:/Progra~1/NVIDIA~2/CUDA/v11.7/bin/nvcc.exe)
>>>>>>> Stashed changes

set(SOURCES
	PoolURI.cpp PoolURI.h
	PoolClient.h
	PoolManager.h PoolManager.cpp
	testing/SimulateClient.h testing/SimulateClient.cpp
	stratum/EthStratumClient.h stratum/EthStratumClient.cpp
	getwork/EthGetworkClient.h getwork/EthGetworkClient.cpp
	C:/msys64/ucrt64/include/limits.h
)

<<<<<<< Updated upstream
add_library(poolprotocols ${SOURCES})
target_link_libraries(poolprotocols PRIVATE devcore ethminer-buildinfo ethash::ethash  ${Boost_LIBRARY_DIR_RELEASE} jsoncpp_static ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
include_directories(AFTER .. ${OPENSSL_INCLUDE_DIR})
=======
#hunter_add_package(OpenSSL)
set(OPENSSL_ROOT_DIR C:/msys64/usr/local)
set(OPENSSL_INCLUDE_DIR C:/msys64/usr/local/include)
set(OPENSSL_LIBRARIES C:/msys64/usr/local/lib)

add_library(poolprotocols ${SOURCES})
target_link_directories(poolprotocols
PRIVATE
	${CMAKE_BINARY_DIR}/../lib
)
target_link_libraries(poolprotocols
PRIVATE
	devcore
	ethminer-buildinfo
	# C:/msys64/usr/lib/libjsoncpp.a
	C:/msys64/usr/local/lib/ethash.lib 
	C:/msys64/usr/local/lib/ethash-global-context.lib 
	C:/msys64/usr/local/lib/libssl.a
	C:/msys64/usr/local/lib/libcrypto.a
	crypt32.lib
)
target_include_directories(poolprotocols
PUBLIC
	C:/progra~2/wi3cf2~1/10/Include/10.0.22621.0/um
PRIVATE 
	${CMAKE_BINARY_DIR}
	${OPENSSL_INCLUDE_DIR}
	C:/msys64/ucrt64/include/ssp
	# C:/msys64/usr/include/json
	C:/msys64/usr/include/ethash
	C:/Progra~1/NVIDIA~2/CUDA/v11.7/include
	${CMAKE_BINARY_DIR}/../
)
>>>>>>> Stashed changes
