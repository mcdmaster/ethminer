file(GLOB HEADERS "*.h")
file(GLOB SOURCES "*.cpp")

add_library(devcore ${SOURCES} ${HEADERS})
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
target_link_libraries(devcore PUBLIC ${Boost_LIBRARY_DIR})
target_link_libraries(devcore PRIVATE ${Threads_LIBRARY_DIR})
target_include_directories(devcore PRIVATE .. ${Boost_INCLUDE_DIR})
