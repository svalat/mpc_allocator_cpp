set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_library(alloc_json STATIC JsonState.cpp TypeToJson.cpp)