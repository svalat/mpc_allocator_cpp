######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
my_enable_fpic()
add_library(alloc_posix OBJECT PosixAllocator.cpp PosixAllocatorDebugTrace.cpp PosixAllocatorFileTrace.cpp PosixAllocatorStd.cpp PosixAllocatorLocal.cpp)
add_library(alloc_posix_export OBJECT PosixExport.cpp)

