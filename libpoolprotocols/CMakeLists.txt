set(CMAKE_C_COMPILER ${CUDA_HOST_COMPILER})
set(CMAKE_CXX_COMPILER ${CUDA_CLANG_COMPILER})

set(SOURCES
	PoolURI.cpp PoolURI.h
	PoolClient.h
	PoolManager.h PoolManager.cpp
	testing/SimulateClient.h testing/SimulateClient.cpp
	stratum/EthStratumClient.h stratum/EthStratumClient.cpp
	getwork/EthGetworkClient.h getwork/EthGetworkClient.cpp
)

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
	ethash::ethash 
	ethash::ethash-global-context 
	OpenSSL::SSL
	OpenSSL::Crypto
	crypt32.lib
)
target_include_directories(poolprotocols
PRIVATE 
	${CMAKE_BINARY_DIR}
	${OPENSSL_INCLUDE_DIR}
	C:/msys64/ucrt64/include/ssp
	# C:/msys64/usr/include/json
	C:/msys64/usr/include/ethash
	${CUDAToolkit_INCLUDE_DIRS}
	${CMAKE_BINARY_DIR}/../
)
