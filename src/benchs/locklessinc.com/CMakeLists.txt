######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_executable(t-test1-no-use-mem t-test1.c)
target_link_libraries(t-test1-no-use-mem ${CMAKE_THREAD_LIBS_INIT})

######################################################
add_executable(t-test1-use-mem t-test1.c)
target_link_libraries(t-test1-use-mem ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(t-test1-use-mem PROPERTIES COMPILE_FLAGS "-DDO_MEMSET")

######################################################
set(MPC_ALLOC_PATH_FOR_BENCH $<alloc>)
configure_file(run.sh.in run.sh @ONLY)

######################################################
add_executable(t-test1-no-use-mem-mpcalloc t-test1.c)
target_link_libraries(t-test1-no-use-mem-mpcalloc ${CMAKE_THREAD_LIBS_INIT} alloc)