######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
set(local_srcs RegionRegistry.cpp Debug.cpp MPSCFQueue.cpp Spinlock.cpp Common.cpp)

######################################################
my_enable_fpic()

######################################################
add_library(alloc_common_internal OBJECT ${local_srcs})

######################################################
add_library(alloc_common OBJECT ${local_srcs})
set_target_properties(alloc_common PROPERTIES COMPILE_FLAGS ${DLL_VISIBILITY_TWEAK})