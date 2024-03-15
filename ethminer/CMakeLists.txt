cmake_policy(SET CMP0015 NEW)
<<<<<<< Updated upstream

aux_source_directory(. SRC_LIST)

# target_include_directories("${CMAKE_SOURCE_DIR}"  "${BUILD_OPENSSL}/include")
include_directories(BEFORE ..  ${OPENSSL_INCLUDE_DIR})

set(EXECUTABLE ethminer)

file(GLOB HEADERS "*.h")

add_executable(${EXECUTABLE} ${SRC_LIST} ${HEADERS})
if(MSVC)
	target_sources(${EXECUTABLE} PRIVATE ethminer.rc)
endif()

hunter_add_package(CLI11)
find_package(CLI11 CONFIG REQUIRED)

target_link_libraries(ethminer PRIVATE ethcore poolprotocols devcore ethminer-buildinfo CLI11::CLI11 ${Boost_LIBRARY_DIR_RELEASE} ${OPENSSL_CRYPTO_LIBRARY} ${OPENSSL_SSL_LIBRARY})
=======
cmake_policy(SET CMP0074 NEW)

hunter_add_package(CLI11)
find_package(CLI11 REQUIRED)
>>>>>>> Stashed changes

include_directories(
	C:/progra~2/wi3cf2~1/10/Include/10.0.22621.0/ucrt
	${CMAKE_BINARY_DIR}
	${CLI11_ROOT}/include
	C:/msys64/ucrt64/include/ssp
	# C:/msys64/usr/include/json
	C:/msys64/usr/local/include
	C:/Progra~1/NVIDIA~2/CUDA/v11.7/include
	${CMAKE_BINARY_DIR}/../
    # ${HEADERS}
)
link_libraries(
	Boost::filesystem
	ucrt.lib
	crypt32.lib
	C:/msys64/usr/local/lib/libssl.a
	C:/msys64/usr/local/lib/libcrypto.a
	# C:/msys64/usr/lib/libjsoncpp.a
	C:/msys64/usr/local/lib/ethash.lib
	C:/msys64/usr/local/lib/ethash-global-context.lib
	C:/msys64/usr/local/lib/keccak.lib
	C:/Progra~1/NVIDIA~2/CUDA/v11.7/lib/x64/cudart_static.lib
	C:/Progra~1/NVIDIA~2/CUDA/v11.7/lib/x64/OpenCL.lib
	${CMAKE_BINARY_DIR}/libethcore/Release/ethcore.lib
	${CMAKE_BINARY_DIR}/libethash-cl/Release/ethash-cl.lib 
	${CMAKE_BINARY_DIR}/libethash-cuda/Release/ethash-cuda.lib 
	${CMAKE_BINARY_DIR}/libpoolprotocols/Release/poolprotocols.lib 
	${CMAKE_BINARY_DIR}/libdevcore/Release/devcore.lib
)
if(ETHDBUS)
	pkg_check_modules(DBUS dbus-1)
	include_directories(${DBUS_INCLUDE_DIRS})
	link_directories(${DBUS_LIBRARY_DIRS})
	link_libraries(${DBUS_LIBRARIES})
endif()
if(APICORE)
	link_libraries(${CMAKE_BINARY_DIR}/libapicore/Release/apicore.lib)
endif()

include(GNUInstallDirs)
add_library(objlib OBJECT main.cpp)
add_executable(ethminer $<TARGET_OBJECTS:objlib>)

install(TARGETS ethminer DESTINATION ${CMAKE_INSTALL_BINDIR})
if(MSVC)
    install(FILES $<TARGET_PDB_FILE:ethminer> DESTINATION ${CMAKE_INSTALL_BINDIR} OPTIONAL)
endif()

# file(GLOB SRC_LIST "*.c" "*.cpp" "*.cu")
# if(MSVC)
#    set(APPEND ${SRC_LIST} ethminer.rc)
# endif()
# file(GLOB HEADERS "*.h" "*.hpp" "*.cuh")

# target_link_libraries(${EXECUTABLE}
# PRIVATE
#	$<PATH:RELATIVE_PATH,./Release/ethminer,${CMAKE_CURRENT_BINDIR}>
# )
