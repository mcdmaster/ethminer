set(SOURCES
    ApiServer.h ApiServer.cpp
	C:/msys64/ucrt64/include/limits.h
)

add_library(apicore ${SOURCES})
target_link_libraries(apicore PRIVATE ethcore devcore ethminer-buildinfo ${Boost_LIBRARY_DIR_RELEASE} ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
include_directories(.. ${SOURCES} ${CMAKE_PREFIX_PATH})
target_link_libraries(apicore
PRIVATE 
    ethcore 
    devcore
    ethminer-buildinfo 
    # C:/msys64/usr/lib/libjsoncpp.a
    Boost::filesystem
)
target_include_directories(apicore
PUBLIC
    C:/progra~2/wi3cf2~1/10/Include/10.0.22621.0/um BEFORE
PRIVATE
    ..
	C:/msys64/ucrt64/include/ssp
	# C:/msys64/usr/include/json
	C:/msys64/usr/local/include
	C:/Progra~1/NVIDIA~2/CUDA/v11.7/include
)
