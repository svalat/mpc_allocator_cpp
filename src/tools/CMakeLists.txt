######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(trace TraceReader.cpp CollisionChecker.cpp EventDispatch.cpp EventDispatchPrint.cpp EventDispatchReplay.cpp BugReproducer.cpp)

######################################################
add_executable(trace_reader reader.cpp)
target_link_libraries(trace_reader trace mpc_alloc_base_internal)

######################################################
add_executable(trace_replay replay.cpp)
target_link_libraries(trace_replay trace mpc_alloc_base_internal)

######################################################
add_executable(trace_reproduce reproduce.cpp)
target_link_libraries(trace_reproduce trace mpc_alloc_base_internal)
