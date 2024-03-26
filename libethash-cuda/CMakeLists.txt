find_package(CUDAToolkit REQUIRED)

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
	if (CUDA_VERSION VERSION_LESS 11.0)
		list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_30,code=sm_30")
	endif()
	if (CUDA_VERSION VERSION_LESS 12.0)
		list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_35,code=sm_35")
	endif()
	# list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_30,code=sm_30")
	list(APPEND CUDA_NVCC_FLAGS "-gencode arch=compute_35,code=sm_35")
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

add_library(ethash-cuda
	${sources}
	${headers}
)

target_link_libraries(ethash-cuda
PRIVATE
	${Boost_LIBRARY_DIR}
PUBLIC
	${CUDAToolkit_LIBRARY}
	${CODA_CLANG_COMPILER}/../lib
)

target_link_libraries(ethash-cuda
PUBLIC
	crypt32.lib
PRIVATE
	$<TARGET_OBJECTS:ethcore>
)

target_include_directories(ethash-cuda
PUBLIC
	${CUDAToolkit_INCLUDE_DIRS}
PRIVATE
	..
	${CMAKE_CURRENT_BINARY_DIR}
	${ethash_INCLIDE_DIR}
)