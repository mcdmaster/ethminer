# A custom command and target to turn the OpenCL kernel into a byte array header
# The normal build depends on it properly and if the kernel file is changed, then
# a rebuild of libethash-cl should be triggered

#TODO: clean up the copy&pasting here

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

<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
if(USE_SYS_OPENCL)
	# On macOS or using ROCm-OpenCL, use system OpenCL library.
	find_package(OpenCL REQUIRED)
else()
	hunter_add_package(OpenCL)
	find_package(OpenCL CONFIG REQUIRED)
endif()
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
#if(APPLE)
#	# On macOS use system OpenCL library.
#	find_package(OpenCL REQUIRED)
#else()
#	hunter_add_package(OpenCL)
#	find_package(OpenCL CONFIG REQUIRED)
#endif()
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

include_directories(
PUBLIC
	C:/progra~2/wi3cf2~1/10/Include/10.0.22621.0/um
PRIVATE
	..
	${CMAKE_CURRENT_BINARY_DIR}
	C:/msys64/ucrt64/include/ssp
	# C:/msys64/usr/include/json
	C:/msys64/usr/include/ethash
	C:/Progra~1/NVIDIA~2/CUDA/v11.7/include
)
set(OPENCL_ROOT_DIR C:/msys64/ucrt64/share/cmake/OpenCLICDLoader)
find_package(OpenCL REQUIRED)

add_library(ethash-cl ${SOURCES})
<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
target_link_libraries(ethash-cl PUBLIC ethcore ethash::ethash)
target_link_libraries(ethash-cl PRIVATE OpenCL::OpenCL)
target_link_libraries(ethash-cl PRIVATE ${Boost_LIBRARY_DIR_RELEASE})
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
target_link_directories(ethash-cl
PRIVATE
	${BOOST_ROOT}/lib
)
target_link_libraries(ethash-cl 
PUBLIC 
	ethcore
	# C:/msys64/usr/lib/libjsoncpp.a
	C:/msys64/usr/local/lib/ethash.lib 
	C:/msys64/usr/local/lib/ethash-global-context.lib 
	C:/msys64/usr/local/lib/libssl.a
	C:/msys64/usr/local/lib/libcrypto.a
PRIVATE
	OpenCL::OpenCL
	Threads::Threads
	Boost::filesystem
)
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
