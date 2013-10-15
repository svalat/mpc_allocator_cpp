######################################################
add_library(trace TraceReader.cpp CollisionChecker.cpp EventDispatch.cpp EventDispatchPrint.cpp EventDispatchReplay.cpp)

######################################################
add_executable(trace_reader reader.cpp)
target_link_libraries(trace_reader alloc_base trace)

######################################################
add_executable(trace_replay replay.cpp)
target_link_libraries(trace_replay alloc_base trace)