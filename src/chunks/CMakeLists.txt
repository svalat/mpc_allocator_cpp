set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(alloc_medium STATIC MediumAllocator.cpp MediumChunk.cpp MediumFreePool.cpp)