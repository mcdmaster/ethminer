# will not use
hunter_config(CURL VERSION ${HUNTER_CURL_VERSION} CMAKE_ARGS HTTP_ONLY=ON CMAKE_USE_OPENSSL=OFF CMAKE_USE_LIBSSH2=OFF CURL_CA_PATH=none)
<<<<<<< Updated upstream
hunter_config(Boost VERSION ${HUNTER_Boost_VERSION} CMAKE_ARGS Boost_Boost=ON Boost_FILESYSTEM=ON Boost_THREAD=ON Boost_MULTITHREADED=ON)
=======
hunter_config(OpenSSL
    VERSION 3.0.5
	URL https://www.openssl.org/source/openssl-3.0.5.tar.gz
	SHA1 a5305213c681a5a4322dad7347a6e66b7b6ef3c7
	CMAKE_ARGS ASM_SUPPORT=ON OPENSSL_MSVC_STATIC_RT=ON CMAKE_BUILD_TYPE=Release CMAKE_CXX_ABI_COMPILED=OFF CMAKE_C_ABI_COMPILED=OFF
)
hunter_config(CLI11 VERSION 1.9.1)
hunter_config(Boost 
	VERSION ${HUNTER_Boost_VERSION}
	# URL https://boostorg.jfrog.io/artifactory/main/release/1.80.0/source/boost_1_80_0.7z
	# SHA1 da39a3ee5e6b4b0d3255bfef95601890afd80709
	CMAKE_ARGS
		Boost_USE_DEBUG_LIBS=OFF
		Boost_USE_RELEASE_LIBS=ON
		Boost_USE_STATIC_LIBS=ON
		Boost_LIB_DIAGNOSTIC_DEFINITIONS=ON
)
>>>>>>> Stashed changes
