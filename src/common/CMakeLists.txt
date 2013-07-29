set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(alloc_common OBJECT RegionRegistry.cpp Spinlock.cpp Debug.cpp MPSCFQueue.cpp)