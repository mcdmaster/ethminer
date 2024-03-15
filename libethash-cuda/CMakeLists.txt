find_package(CUDAToolkit REQUIRED)

set(CUDA_HOST_COMPILER C:/progra~1/micros~2/2022/Preview/VC/Tools/MSVC/14.34.31721/bin/Hostx64/x64/cl.exe)
set(CUDA_CLANG_COMPILER C:/msys64/opt/bin)

# set(VCInstallDir C:/progra~1/micros~2/2022/Preview/VC/)
# set(VCToolsVersion 14.34.31721)
# set(Platform x64)
# set(PlatformTarget x64)
set(CUDA_NVCC_FLAGS
	${CUDA_NVCC_FLAGS};--ptxas-options=-v;-use_fast_math;-ccbin;${CUDA_HOST_COMPILER};-Xcompiler;"/D \"-static\" /EHsc /W3 /nologo /O2 /Zi /MT";-forward-unknown-to-host-compiler
)
if (NOT MSVC)
	list(APPEND CUDA_NVCC_FLAGS "--disable-warnings")
endif()

list(APPEND CUDA_NVCC_FLAGS_RELEASE -O3)
list(APPEND CUDA_NVCC_FLAGS_DEBUG -G)

if(COMPUTE AND (COMPUTE GREATER 0))
	list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_${COMPUTE},code=sm_${COMPUTE}")
else()
<<<<<<< Updated upstream
	if (CUDA_VERSION VERSION_LESS 11.0)
		list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_30,code=sm_30")
	endif()
	if (CUDA_VERSION VERSION_LESS 12.0)
		list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_35,code=sm_35")
	endif()
=======
	# list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_30,code=sm_30")
	list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_35,code=sm_35")
>>>>>>> Stashed changes
	list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_50,code=sm_50")
	list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_52,code=sm_52")
	list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_53,code=sm_53")
	list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_60,code=sm_60")
	list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_61,code=sm_61")
	list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_62,code=sm_62")
	if(NOT CUDA_VERSION VERSION_LESS 9.0)
		list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_70,code=sm_70")
	endif()
	if(NOT CUDA_VERSION VERSION_LESS 10.0)
		list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_75,code=sm_75")
	endif()
	if(NOT CUDA_VERSION VERSION_LESS 11.0)
		# NVIDIA A100 and NVIDIA DGX-A100
		list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_80,code=sm_80")
	endif()
	if(NOT CUDA_VERSION VERSION_LESS 11.1)
		# Tesla GA10x cards, RTX Ampere – RTX 3080/3090, RTX A6000, RTX A40
		list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_86,code=sm_86")
	endif()
endif()

file(GLOB sources "*.cpp" "*.cu")
file(GLOB headers "*.h" "*.cuh")

<<<<<<< Updated upstream
cuda_add_library(ethash-cuda STATIC ${sources} ${headers})
target_link_libraries(ethash-cuda ethcore ethash::ethash  ${Boost_LIBRARY_DIR_RELEASE})
target_include_directories(ethash-cuda PUBLIC ${CUDA_INCLUDE_DIRS})
target_include_directories(ethash-cuda PRIVATE .. ${CMAKE_CURRENT_BINARY_DIR})
=======
add_library(ethash-cuda STATIC ${sources} ${headers})

target_link_libraries(ethash-cuda
PUBLIC
	cudart_static
	nvrtc_static
	nvrtc-builtins_static
PRIVATE
	ethcore
	Threads::Threads
	# C:/msys64/usr/lib/libjsoncpp.a
	C:/msys64/usr/local/lib/ethash.lib 
	C:/msys64/usr/local/lib/ethash-global-context.lib 
	C:/msys64/usr/local/lib/libssl.a
	C:/msys64/usr/local/lib/libcrypto.a
	crypt32.lib
)
target_link_directories(ethash-cuda
PUBLIC
	${CUDAToolkit_LIBRARY_DIR}
	${CODA_CLANG_COMPILER}/../lib
)
target_include_directories(ethash-cuda
PUBLIC
	${CUDAToolkit_INCLUDE_DIRS}
PRIVATE
	..
	${CMAKE_CURRENT_BINARY_DIR}
	# C:/msys64/usr/include/json
	C:/msys64/usr/include/ethash
)
>>>>>>> Stashed changes
