set(SOURCES
    ApiServer.h ApiServer.cpp
	C:/msys64/ucrt64/include/limits.h
)

find_package(OpenSSL REQUIRED)
find_package(ethash REQUIRED)
find_package(Boost REQUIRED)
add_library(apicore ${SOURCES})
target_link_libraries(apicore
PRIVATE 
    ethcore 
    devcore
    ethminer-buildinfo 
)
include_directories(.. ${SOURCES} ${CMAKE_PREFIX_PATH})
target_include_directories(apicore
PUBLIC
    C:/progra~2/wi3cf2~1/10/Include/10.0.22621.0/um BEFORE
PRIVATE
    ..
	C:/msys64/ucrt64/include/ssp
	# C:/msys64/usr/include/json
	C:/msys64/usr/local/include
	C:/Progra~1/NVIDIA~2/CUDA/v12.3/include
)
