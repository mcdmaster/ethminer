file(GLOB HEADERS "*.h")
file(GLOB SOURCES "*.cpp")

add_library(devcore ${SOURCES} ${HEADERS})
target_link_libraries(devcore PUBLIC Boost::boost)
target_link_libraries(devcore PRIVATE Threads::Threads)

