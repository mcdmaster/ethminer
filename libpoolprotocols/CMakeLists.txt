cmake_minimum_required(VERSION 3.24)
project(poolprotocols)

set(SOURCES
	PoolURI.cpp PoolURI.h
	PoolClient.h
	PoolManager.h PoolManager.cpp
	testing/SimulateClient.h testing/SimulateClient.cpp
	stratum/EthStratumClient.h stratum/EthStratumClient.cpp
	getwork/EthGetworkClient.h getwork/EthGetworkClient.cpp
	C:/msys64/ucrt64/include/limits.h
)

add_library(poolprotocols ${SOURCES})
hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)

target_link_libraries(poolprotocols
PRIVATE
	devcore
	ethminer-buildinfo
	jsoncpp_static
	ethash::ethash
	OpenSSL:ssl
	OpenSSL:crypto
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