######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
set(local_srcs MediumAllocator.cpp MediumChunk.cpp MediumFreePool.cpp SmallChunkRun.cpp SmallAllocator.cpp SmallRunContainer.cpp HugeAllocator.cpp)

######################################################
my_enable_fpic()

######################################################
add_library(alloc_medium_internal OBJECT ${local_srcs})

######################################################
add_library(alloc_medium OBJECT ${local_srcs})
set_target_properties(alloc_medium PROPERTIES COMPILE_FLAGS ${DLL_VISIBILITY_TWEAK})