#Resuire 2.8.9 Due to use of CMAKE_POSITION_INDEPENDENT_CODE
cmake_minimum_required(VERSION 2.8.9)
project(mpc_allocator_cpp)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories(common medium json mmsource)

find_package (Threads REQUIRED)

file(WRITE ${CMAKE_BINARY_DIR}/dummy.c "")

add_library(alloc SHARED ${CMAKE_BINARY_DIR}/dummy.c)
target_link_libraries(alloc alloc_mmsource alloc_medium alloc_json alloc_common)
target_link_libraries(alloc ${CMAKE_THREAD_LIBS_INIT})

add_executable(mpc_allocator_cpp main.cpp)
target_link_libraries(mpc_allocator_cpp alloc)

install(TARGETS mpc_allocator_cpp RUNTIME DESTINATION bin)

add_subdirectory(common)
add_subdirectory(json)
add_subdirectory(mmsource)
add_subdirectory(medium)
add_subdirectory(tests)
