file(GLOB HEADERS "*.h")
file(GLOB SOURCES "*.cpp")

add_library(devcore ${SOURCES} ${HEADERS})
<<<<<<< Updated upstream
target_link_libraries(devcore PUBLIC ${Boost_LIBRARY_DIR_RELEASE})
=======
target_link_libraries(devcore PUBLIC Boost::boost)
>>>>>>> Stashed changes
target_link_libraries(devcore PRIVATE Threads::Threads)
target_include_directories(devcore PRIVATE .. ${Boost_INCLUDE_DIR})
