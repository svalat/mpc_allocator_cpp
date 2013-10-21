######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
my_enable_fpic()
add_library(alloc_portability OBJECT OS${PORTABILITY_OS}.cpp Topo${PORTABILITY_TOPO}.cpp)
