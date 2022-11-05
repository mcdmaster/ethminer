set(SOURCES
    ApiServer.h ApiServer.cpp
)

add_library(apicore ${SOURCES})
target_link_libraries(apicore
PRIVATE 
    ethcore 
    devcore
    ethminer-buildinfo 
    # C:/msys64/usr/lib/libjsoncpp.a
    Boost::filesystem
)
target_include_directories(apicore
PRIVATE
    ..
	C:/msys64/ucrt64/include/ssp
	# C:/msys64/usr/include/json
	C:/msys64/usr/local/include
	${CUDAToolkit_INCLUDE_DIRS}
)
