set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(alloc_medium OBJECT MediumAllocator.cpp MediumChunk.cpp MediumFreePool.cpp SmallChunkRun.cpp SmallAllocator.cpp PaddedChunk.cpp)