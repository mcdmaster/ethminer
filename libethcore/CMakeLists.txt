set(SOURCES
	EthashAux.h EthashAux.cpp
	Farm.cpp Farm.h
	Miner.h Miner.cpp
)

include_directories(BEFORE
	..
	# C:/msys64/usr/include/json
	C:/msys64/usr/include/ethash
)

add_library(ethcore ${SOURCES})
target_link_libraries(ethcore 
PUBLIC 
devcore 
PRIVATE
hwmon
# C:/msys64/usr/lib/libjsoncpp.a
	ethash::ethash 
	ethash::ethash-global-context 
	crypt32.lib
)

if(ETHASHCL)
	target_link_libraries(ethcore PRIVATE ethash-cl)
endif()
if(ETHASHCUDA)
	target_link_libraries(ethcore PUBLIC ethash-cuda)
endif()
if(ETHASHCPU)
	target_link_libraries(ethcore PUBLIC ethash-cpu)
endif()
