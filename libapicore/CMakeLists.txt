set(SOURCES
    ApiServer.h ApiServer.cpp
)

add_library(apicore ${SOURCES})
target_link_libraries(apicore PRIVATE ethcore devcore ethminer-buildinfo ${Boost_LIBRARY_DIR_RELEASE} ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
include_directories(.. ${SOURCES} ${CMAKE_PREFIX_PATH})
