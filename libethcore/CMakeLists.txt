set(SOURCES
	EthashAux.h EthashAux.cpp
	Farm.cpp Farm.h
	Miner.h Miner.cpp
)

<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
include_directories(BEFORE ..  ${Boost_INCLUDE_DIR})
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
include_directories(BEFORE
	..
	# C:/msys64/usr/include/json
	C:/msys64/usr/include/ethash
)
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

add_library(ethcore ${SOURCES})
target_link_libraries(ethcore 
PUBLIC 
devcore 
PRIVATE
hwmon
# C:/msys64/usr/lib/libjsoncpp.a
C:/msys64/usr/local/lib/ethash.lib 
C:/msys64/usr/local/lib/ethash-global-context.lib 
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
