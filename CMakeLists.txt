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
#Otherwise it may be 2.7 or 2.6 or something similar
cmake_minimum_required(VERSION 2.8.9)
project(mpc_allocator_cpp)
set(ALLOC_VERSION "0.0.0-dev")
set(ALLOC_API_VERSION 1)

######################################################
#global setup
enable_testing()
add_definitions(-DVERSION="${ALLOC_VERSION}")
add_definitions(-D_GNU_SOURCE)
add_definitions(-DHAVE_COLOR)

######################################################
set(DLL_VISIBILITY_TWEAK "-fno-rtti -fno-exceptions -fvisibility=hidden -fvisibility-inlines-hidden -DENABLE_VISIBILITY_TWEAK -DBUILDING_DLL")

######################################################
#Add internal search path for scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/)

######################################################
#Delegate some cmake scripts
include(${CMAKE_SOURCE_DIR}/cmake/macros.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/enable-options.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/cpack-options.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/check-deps.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/user-options.cmake)

######################################################
#some include dirs
include_directories(src/lib/common)
include_directories(src/lib/chunks)
include_directories(src/lib/json)
include_directories(src/lib/mmsource)
include_directories(src/lib/portability)
include_directories(src/lib/posix)
include_directories(extern/include)
#some external include dirs
include_directories(${HWLOC_INCLUDE_DIRS})
include_directories(${LIBDL_INCLUDE_DIRS})

######################################################
#manage subdirs
add_subdirectory(src)
