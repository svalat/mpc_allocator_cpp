cmake_minimum_required(VERSION 2.8)
project(mpc_allocator_cpp)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories(common medium json)

add_library(alloc medium/MediumFreePool.cpp medium/MediumChunk.cpp common/Spinlock.cpp medium/MediumAllocator.cpp common/RegionRegistry.cpp common/OSLinux.cpp json/JsonState.cpp json/TypeToJson.cpp)
add_executable(mpc_allocator_cpp main.cpp)
target_link_libraries(mpc_allocator_cpp alloc)

install(TARGETS mpc_allocator_cpp RUNTIME DESTINATION bin)

add_subdirectory(tests)
