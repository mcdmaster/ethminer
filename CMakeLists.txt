set(CMP0074 NEW)
set(CMP0079 NEW)
set(CMP0093 NEW)
set(CMP0135 NEW)
cmake_minimum_required(VERSION 3.5)

set(DOWNLOAD_EXTRACT_TIMESTAMP true)
include(cmake/cable/bootstrap.cmake)

include(CableBuildInfo)
include(CableBuildType)
include(CableToolchains)
include(HunterGate)

include(defaults/HunterCacheServers)

cable_configure_toolchain(DEFAULT cxx11)

set(HUNTER_CONFIGURATION_TYPES Release CACHE STRING "Build type of Hunter packages")
set(HUNTER_JOBS_NUMBER 6 CACHE STRING "Number of parallel builds used by Hunter")
HunterGate(
    URL https://github.com/cpp-pm/hunter/archive/v0.24.3.tar.gz
    SHA1 10738b59e539818a01090e64c2d09896247530c7
    LOCAL
)

cable_set_build_type(DEFAULT Release CONFIGURATION_TYPES Release RelWithDebInfo)
project(ethminer)
set(PROJECT_VERSION 0.19.0)

option(ETHASHCL "Build with OpenCL mining" ON)
option(ETHASHCUDA "Build with CUDA mining" ON)
option(ETHASHCPU "Build with CPU mining (only for development)" OFF)
option(ETHDBUS "Build with D-Bus support" OFF)
option(APICORE "Build with API Server support" ON)
option(BINKERN "Install AMD binary kernels" ON)
option(DEVBUILD "Log developer metrics" OFF)

# propagates CMake configuration options to the compiler
function(configureProject)
    if (ETHASHCL)
        add_definitions(-DETH_ETHASHCL)
    endif()
    if (ETHASHCUDA)
        add_definitions(-DETH_ETHASHCUDA)
    endif()
    if (ETHASHCPU)
        add_definitions(-DETH_ETHASHCPU)
    endif()
    if (ETHDBUS)
        add_definitions(-DETH_DBUS)
    endif()
    if (APICORE)
        add_definitions(-DAPI_CORE)
    endif()
    if (BINKERN)
        add_definitions(-DBIN_KERN)
    endif()
    if (DEVBUILD)
        add_definitions(-DDEV_BUILD)
    endif()
endfunction()

find_package(PkgConfig)
hunter_add_package(Boost COMPONENTS filesystem thread)
find_package(Boost CONFIG REQUIRED COMPONENTS filesystem thread)
set(CMAKE_THREAD_LIBS_INIT Boost::thread)
find_package(Threads REQUIRED)

hunter_add_package(ethash)
find_package(ethash CONFIG REQUIRED)

configureProject()

message("----------------------------------------------------------------------------")
message("-- CMake ${CMAKE_VERSION}")
message("-- Build ${CMAKE_BUILD_TYPE} / ${CMAKE_SYSTEM_NAME}")
message("----------------------------------------------------------------- components")
message("-- ETHASHCL         Build OpenCL components                      ${ETHASHCL}")
message("-- ETHASHCUDA       Build CUDA components                        ${ETHASHCUDA}")
message("-- ETHASHCPU        Build CPU components (only for development)  ${ETHASHCPU}")
message("-- ETHDBUS          Build D-Bus components                       ${ETHDBUS}")
message("-- APICORE          Build API Server components                  ${APICORE}")
message("-- BINKERN          Install AMD binary kernels                   ${BINKERN}")
message("-- DEVBUILD         Build with dev logging                       ${DEVBUILD}")
message("----------------------------------------------------------------------------")
message("")

include(EthCompilerSettings)
if(UNIX AND NOT APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libstdc++")
endif()

cable_add_buildinfo_library(PROJECT_NAME ${PROJECT_NAME})

add_subdirectory(ethminer)
add_subdirectory(libdevcore)
add_subdirectory(libethcore)
add_subdirectory(libhwmon)
add_subdirectory(libpoolprotocols)

if (ETHASHCL)
    add_subdirectory(libethash-cl)
    if (BINKERN)
        add_subdirectory(libethash-cl/kernels)
    endif()
    target_link_directories(ethminer PRIVATE ${CMAKE_BINARY_DIR}/libethash-cl/Release/ethash-cl.lib)
endif ()
if (ETHASHCUDA)
    add_subdirectory(libethash-cuda)
    target_link_directories(ethminer PRIVATE ${CMAKE_BINARY_DIR}/libethash-cuda/Release/ethash-cuda.lib)
endif ()
if (ETHASHCPU)
    add_subdirectory(libethash-cpu)
endif ()
if (APICORE)
    add_subdirectory(libapicore)
    target_link_directories(ethminer PRIVATE ${CMAKE_BINARY_DIR}/libapicore/Release/apicore.lib)
endif()

if(WIN32)
    set(CPACK_GENERATOR ZIP)
else()
    set(CPACK_GENERATOR TGZ)
endif()
set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_CHECKSUM SHA256)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY FALSE)
include(CPack)

target_include_directories(ethminer
PRIVATE
    ${CMAKE_BINARY_DIR}
	# C:/msys64/usr/include/json
	# C:/msys64/ucrt64/include/ssp
	# C:/msys64/usr/local/include
	# C:/msys64/ucrt64/include
    ${CMAKE_BINARY_DIR}/../
)
target_link_directories(ethminer 
PRIVATE
    ${CMAKE_BINARY_DIR}
    C:/msys64/usr/lib
    C:/msys64/usr/local/lib
	${CMAKE_BINARY_DIR}/libethcore/Release/ethcore.lib
	${CMAKE_BINARY_DIR}/libpoolprotocols/Release/poolprotocols.lib 
	${CMAKE_BINARY_DIR}/libdevcore/Release/devcore.lib
    ${CMAKE_BINARY_DIR}/../
)
