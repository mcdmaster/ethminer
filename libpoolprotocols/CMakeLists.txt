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

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED Threads::Threads)
LINK_LIBRARIES(${CMAKE_THREAD_LIBS_INIT})

find_package(OpenSSL REQUIRED)

find_package(CUDA REQUIRED)

target_link_libraries(poolprotocols
PRIVATE
	devcore
	ethminer-buildinfo
)
target_include_directories(poolprotocols
PUBLIC $ENV{WINDOWSSDK_INCLUDEPATH}
PRIVATE 
	${CMAKE_BINARY_DIR}
	${OPENSSL_INCLUDE_DIR}
	C:/msys64/ucrt64/include/ssp
	# C:/msys64/usr/include/json
	C:/msys64/usr/include/ethash
	C:/Progra~1/NVIDIA~2/CUDA/v12.3/include
	${CMAKE_BINARY_DIR}/../
)