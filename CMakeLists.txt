set(CMP0074 NEW)
set(CMP0079 NEW)
set(CMP0093 NEW)
set(CMP0135 NEW)
cmake_minimum_required(VERSION 3.14)

set(DOWNLOAD_EXTRACT_TIMESTAMP ON)
set(CABLE_ROOT ${CMAKE_BINARY_DIR}/../cmake/cable/)
include(${CABLE_ROOT}bootstrap.cmake)
include(${CABLE_ROOT}CableBuildInfo.cmake)
include(${CABLE_ROOT}CableBuildType.cmake)
include(${CABLE_ROOT}CableToolchains.cmake)
include(${CABLE_ROOT}defaults/HunterCacheServers.cmake)

set(HUNTER_CONFIGURATION_TYPES Release CACHE STRING "Build type of Hunter packages")
set(HUNTER_JOBS_NUMBER 6 CACHE STRING "Number of parallel builds used by Hunter")

include(FetchContent)
cable_configure_toolchain(DEFAULT cxx11)
FetchContent_Declare(SetupHunter GIT_REPOSITORY https://github.com/cpp-pm/gate)

set(HUNTER_CURL_VERSION 7.74.0-p2)
list(APPEND HUNTER_PACKAGES CURL)
set(HUNTER_Boost_VERSION 1.81.0)
list(APPEND HUNTER_PACKAGES Boost)
set(HUNTER_CLI11_VERSION 1.9.1)
list(APPEND HUNTER_PACKAGES CLI11)
set(HUNTER_ethash_VERSION 1.0.0)
list(APPEND HUNTER_PACKAGES ethash)
set(HUNTER_jsoncpp_VERSION 1.9.5-b1)
list(APPEND HUNTER_PACKAGES jsoncpp)
# set(HUNTER_Threads_VERSION 3.1)
# list(APPEND HUNTER_PACKAGES Threads)
set(HUNTER_OpenSSL_VERSION 3.0.8)
list(APPEND HUNTER_PACKAGES OpenSSL)
FetchContent_MakeAvailable(SetupHunter)

project(ethminer)
set(PROJECT_VERSION 0.19.10)
cable_set_build_type(DEFAULT Release CONFIGURATION_TYPES Release RelWithDebInfo)

option(ETHASHCL "Build with OpenCL mining" ON)
option(ETHASHCUDA "Build with CUDA mining" ON)
option(ETHASHCPU "Build with CPU mining (only for development)" OFF)
option(ETHDBUS "Build with D-Bus support" OFF)
option(APICORE "Build with API Server support" ON)
option(BINKERN "Install AMD binary kernels" ON)
option(DEVBUILD "Log developer metrics" OFF)
option(USE_SYS_OPENCL "Build with system OpenCL" ON)

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
    if (USE_SYS_OPENCL)
        add_definitions(-DUSE_SYS_OPENCL)
    endif()
endfunction()

find_program(perl C:/Strawberry/perl/bin/perl5.38.2.exe)

find_package(Boost CONFIG REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED Threads::Threads)
link_libraries(${CMAKE_THREAD_LIBS_INIT})

find_package(jsoncpp REQUIRED)
find_package(ethash REQUIRED)

configureProject()

if(APPLE)
    set(USE_SYS_OPENCL ON)
endif()

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
message("-- USE_SYS_OPENCL   Build with system OpenCL                     ${USE_SYS_OPENCL}")
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
PUBLIC
    C:/progra~2/wi3cf2~1/10/Include/10.0.22621.0/um
PRIVATE
    ${CMAKE_BINARY_DIR}
	# C:/msys64/usr/include/json
	C:/msys64/ucrt64/include/ssp
	C:/msys64/usr/local/include
	C:/msys64/ucrt64/include
    ${CMAKE_BINARY_DIR}/../
)
target_link_directories(ethminer 
PUBLIC
    C:/progra~2/wi3cf2~1/10/lib/10.0.22621.0/x64/um
PRIVATE
    ${CMAKE_BINARY_DIR}
    C:/msys64/usr/lib
    C:/msys64/usr/local/lib
	${CMAKE_BINARY_DIR}/libethcore/Release/ethcore.lib
	${CMAKE_BINARY_DIR}/libpoolprotocols/Release/poolprotocols.lib 
	${CMAKE_BINARY_DIR}/libdevcore/Release/devcore.lib
    ${CMAKE_BINARY_DIR}/../
)
