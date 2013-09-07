######################################################
add_executable(trace_reader TraceReader.cpp reader.cpp)
target_link_libraries(trace_reader alloc_base)

######################################################
add_executable(trace_replay TraceReader.cpp replay.cpp CollisionChecker.cpp)
target_link_libraries(trace_replay alloc_base)