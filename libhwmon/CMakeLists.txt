<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
set(SOURCES
    wraphelper.cpp wraphelper.h
    wrapnvml.h wrapnvml.cpp
    wrapadl.h wrapadl.cpp
    wrapamdsysfs.h wrapamdsysfs.cpp
)

add_library(hwmon ${SOURCES})
set(HWMON_LINK_LIBRARIES devcore)

if (UNIX)
	list(APPEND HWMON_LINK_LIBRARIES dl)
endif ()

target_link_libraries(hwmon ${HWMON_LINK_LIBRARIES})
target_include_directories(hwmon PRIVATE ..  ${Boost_INCLUDE_DIR})

if (ETHASHCUDA)
	find_package(CUDA REQUIRED)
	target_include_directories(hwmon PUBLIC ${CUDA_INCLUDE_DIRS})
endif()
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
set(SOURCES
    wraphelper.cpp wraphelper.h
    wrapnvml.h wrapnvml.cpp
    wrapadl.h wrapadl.cpp
    wrapamdsysfs.h wrapamdsysfs.cpp
)

add_library(hwmon ${SOURCES})
set(HWMON_LINK_LIBRARIES devcore)

if (UNIX)
	list(APPEND HWMON_LINK_LIBRARIES dl)
endif ()

target_link_libraries(hwmon ${HWMON_LINK_LIBRARIES})
target_include_directories(hwmon PRIVATE ..)

if (ETHASHCUDA)
	find_package(CUDA REQUIRED)
	target_include_directories(hwmon PUBLIC ${CUDA_INCLUDE_DIRS})
endif()
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
