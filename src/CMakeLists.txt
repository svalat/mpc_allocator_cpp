######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_subdirectory(tools)
add_subdirectory(lib)

######################################################
if (NOT DISABLE_TESTS)
	add_subdirectory(tests)
endif (NOT DISABLE_TESTS)
