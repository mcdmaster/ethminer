cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0074 NEW)

hunter_add_package(CLI11)
find_package(CLI11 REQUIRED)

include_directories(
	${CMAKE_BINARY_DIR}
	${CLI11_ROOT}/include
	# C:/msys64/usr/include/json
	C:/msys64/usr/local/include
	${CUDAToolkit_INCLUDE_DIRS}
	${CMAKE_BINARY_DIR}/../
    # ${HEADERS}
)
link_libraries(
	Boost::filesystem
	ucrt.lib
	crypt32.lib
	# C:/msys64/usr/lib/libjsoncpp.a
	ethash::ethash 
	ethash::ethash-global-context 
	OpenSSL::SSL
	OpenSSL::Crypto
	${CUDAToolkit_LIBRARY_DIR}
	${CODA_CLANG_COMPILER}/../lib
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
