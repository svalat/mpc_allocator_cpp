######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
#Resuire 2.8.9 Due to use of CMAKE_POSITION_INDEPENDENT_CODE 
#Require 2.8.9 Due to use of OBJECT keyword to build libraries
cmake_minimum_required(VERSION 2.8.9)
project(mpc_allocator_cpp)
set(ALLOC_VERSION "0.0.0-dev")

######################################################
#global setup
enable_testing()
add_definitions(-DVERSION="${ALLOC_VERSION}")
add_definitions(-D_GNU_SOURCE)
add_definitions(-DHAVE_COLOR)

######################################################
#Add internal search path for scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)

######################################################
#Delegate some cmake scripts
include(${CMAKE_SOURCE_DIR}/cmake/enable-options.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cpack-options.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/check-deps.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/user-options.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/macros.cmake)

######################################################
#some include dirs
include_directories(src/common)
include_directories(src/chunks)
include_directories(src/json)
include_directories(src/mmsource)
include_directories(src/portability)
include_directories(src/posix)
include_directories(extern/include)
#some external include dirs
include_directories(${HWLOC_INCLUDE_DIRS})

######################################################
#manage subdirs
add_subdirectory(src)

######################################################
#tmp exec for sandbox tests
add_executable(mpc_allocator_cpp main.cpp)
target_link_libraries(mpc_allocator_cpp alloc)
