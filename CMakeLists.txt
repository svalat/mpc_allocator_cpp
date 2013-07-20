cmake_minimum_required(VERSION 2.8)
project(mpc_allocator_cpp)

find_package(GTest)

add_library(alloc StdPool.cpp Chunk.cpp)
add_executable(mpc_allocator_cpp main.cpp)
target_link_libraries(mpc_allocator_cpp alloc)

install(TARGETS mpc_allocator_cpp RUNTIME DESTINATION bin)

add_subdirectory(tests)