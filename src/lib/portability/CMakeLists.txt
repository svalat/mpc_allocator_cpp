######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
set(local_srcs OS${PORTABILITY_OS}.cpp Topo${PORTABILITY_TOPO}.cpp)

######################################################
my_enable_fpic()

######################################################
add_library(alloc_portability_internal OBJECT ${local_srcs})

######################################################
add_library(alloc_portability OBJECT ${local_srcs})
set_target_properties(alloc_portability PROPERTIES COMPILE_FLAGS ${DLL_VISIBILITY_TWEAK})