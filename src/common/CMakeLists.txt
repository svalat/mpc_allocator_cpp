set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(alloc_common OBJECT RegionRegistry.cpp Debug.cpp MPSCFQueue.cpp Spinlock.cpp)