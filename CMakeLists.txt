#Resuire 2.8.9 Due to use of CMAKE_POSITION_INDEPENDENT_CODE
cmake_minimum_required(VERSION 2.8.9)
project(mpc_allocator_cpp)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package (Threads REQUIRED)

include_directories(src/common src/chunks src/json src/mmsource)

add_executable(mpc_allocator_cpp main.cpp)
target_link_libraries(mpc_allocator_cpp alloc)

add_subdirectory(src)

# install(TARGETS mpc_allocator_cpp RUNTIME DESTINATION bin)
