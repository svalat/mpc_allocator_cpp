set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(alloc_common STATIC OSLinux.cpp RegionRegistry.cpp Spinlock.cpp Debug.cpp)