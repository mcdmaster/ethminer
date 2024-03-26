# A custom command and target to turn the OpenCL kernel into a byte array header
# The normal build depends on it properly and if the kernel file is changed, then
# a rebuild of libethash-cl should be triggered

#TODO: clean up the copy&pasting here

SET(MSVC_VERSION 1939)

find_package(ethash REQUIRED)
find_package(Boost REQUIRED)
add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ethash.h
	COMMAND ${CMAKE_COMMAND} ARGS
	-DBIN2H_SOURCE_FILE="${CMAKE_CURRENT_SOURCE_DIR}/kernels/cl/ethash.cl"
	-DBIN2H_VARIABLE_NAME=ethash_cl
	-DBIN2H_HEADER_FILE="${CMAKE_CURRENT_BINARY_DIR}/ethash.h"
	-P "${CMAKE_CURRENT_SOURCE_DIR}/bin2h.cmake"
	COMMENT "Generating OpenCL Kernel Byte Array"
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/kernels/cl/ethash.cl
)
add_custom_target(clbin2h_stable DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ethash.h ${CMAKE_CURRENT_SOURCE_DIR}/cl/ethash.cl)

set(SOURCES
	CLMiner.h
	CLMiner.cpp
	ethash.h
	C:/msys64/ucrt64/include/limits.h
)

if(USE_SYS_OPENCL)
	# On macOS or using ROCm-OpenCL, use system OpenCL library.
	# find_package(OpenCL REQUIRED)
else()
	hunter_add_package(OpenCL)
endif()

# set(OPENCL_ROOT_DIR C:/msys64/ucrt64/share/cmake/OpenCLICDLoader)
# set(OPENCL_ROOT_DIR C:/Eth/miner/ROCclr)
find_package(CUDAToolkit)
find_package(OpenCL REQUIRED)

include_directories(
PUBLIC
	$ENV{WINDOWSSDK_INCLUDEPATH}cmake
PRIVATE
	..
	${CMAKE_CURRENT_BINARY_DIR}
	C:/msys64/ucrt64/include/ssp
	C:/msys64/usr/include/ethash
	${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
)

add_library(ethash-cl ${SOURCES})
target_link_libraries(ethash-cl
PUBLIC
	${ethash_LIBRARY_DIR}
	${OpemCL_LIBRARY_DIR}
	${BOOST_LIBRARY_DIR}
PRIVATE
	ethcore
)