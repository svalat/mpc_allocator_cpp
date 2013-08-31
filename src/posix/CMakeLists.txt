set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(alloc_posix OBJECT PosixAllocator.cpp PosixAllocatorDebugTrace.cpp)
add_library(alloc_posix_export OBJECT PosixExport.cpp)

