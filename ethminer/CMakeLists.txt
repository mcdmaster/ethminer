cmake_policy(SET CMP0015 NEW)
cmake_policy(SET CMP0074 NEW)

aux_source_directory(. SRC_LIST)

# target_include_directories("${CMAKE_SOURCE_DIR}"  "${BUILD_OPENSSL}/include")
include_directories(BEFORE ..  ${OPENSSL_INCLUDE_DIR})

file(GLOB HEADERS "*.h")
add_executable(ethminer
	${SRC_LIST}
	${HEADERS}
)
find_package(ethash REQUIRED)
find_package(Boost REQUIRED)
find_package(CUDAToolkit REQUIRED)

target_link_libraries(ethminer
PRIVATE 
	ethcore 
	poolprotocols 
	devcore
	ethminer-buildinfo
PUBLIC
	ucrt.lib
	${Boost_LIBRARY_DIR}
	${ethash_LIBRARY_DIR}
	${CUDAToolkit_LIBRARY_DIR}/cudart_static.lib
)
if(MSVC)
	add_library(objlib ethminer.rc)
	if(OpenSSL)
		target_link_libraries(ethminer
		PUBLIC
			${OpenSSL_APPLINK_SOURCE}
			crypt32.lib
		)
	endif()
endif()

target_include_directories(ethminer
PUBLIC
	$ENV{WINDOWSSDK_INCLUDEPATH}
	C:/msys64/ucrt64/include/ssp
	${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
PRIVATE
	${CMAKE_BINARY_DIR}
	${CMAKE_BINARY_DIR}/../
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
target_sources(ethminer PRIVATE main.cpp)
# add_library(objlib
# 	main.cpp
# )
# target_link_libraries(objlib ethminer)

install(TARGETS ethminer DESTINATION ${CMAKE_INSTALL_BINDIR})
if(MSVC)
    install(FILES $<TARGET_PDB_FILE:ethminer> DESTINATION ${CMAKE_INSTALL_BINDIR} OPTIONAL)
endif()

