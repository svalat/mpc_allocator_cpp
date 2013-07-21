cmake_minimum_required(VERSION 2.8)
project(mpc_allocator_cpp)

find_package(GTest)

include_directories(common medium)

add_library(alloc medium/MediumFreePool.cpp medium/MediumChunk.cpp common/Spinlock.cpp medium/MediumAllocator.cpp)
add_executable(mpc_allocator_cpp main.cpp)
target_link_libraries(mpc_allocator_cpp alloc)

install(TARGETS mpc_allocator_cpp RUNTIME DESTINATION bin)

add_subdirectory(tests)