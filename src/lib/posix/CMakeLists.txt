######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
set(local_srcs PosixAllocator.cpp PosixAllocatorDebugTrace.cpp PosixAllocatorFileTrace.cpp PosixAllocatorStd.cpp PosixAllocatorLocal.cpp)

######################################################
my_enable_fpic()

######################################################
add_library(alloc_posix_internal OBJECT ${local_srcs})

######################################################
add_library(alloc_posix OBJECT ${local_srcs})
set_target_properties(alloc_posix PROPERTIES COMPILE_FLAGS ${DLL_VISIBILITY_TWEAK})

######################################################
add_library(alloc_posix_export OBJECT PosixExport.cpp)

