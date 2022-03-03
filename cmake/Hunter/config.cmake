hunter_config(Boost VERSION ${HUNTER_Boost_VERSION})
hunter_config(CURL VERSION ${HUNTER_CURL_VERSION} CMAKE_ARGS HTTP_ONLY=ON CMAKE_USE_OPENSSL=OFF CMAKE_USE_LIBSSH2=OFF CURL_CA_PATH=none)
hunter_config(OpenSSL
  URL https://www.openssl.org/source/openssl-3.0.1.tar.gz
  SHA1 33b00311e7a910f99ff041deebc6dd7bb9f459de
)
hunter_config(OpenCL VERSION ${HUNTER_OpenCL_VERSION})
