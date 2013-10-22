######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
set(local_srcs JsonState.cpp TypeToJson.cpp)

######################################################
my_enable_fpic()

######################################################
add_library(alloc_json_internal OBJECT ${local_srcs})

######################################################
add_library(alloc_json OBJECT ${local_srcs})
set_target_properties(alloc_json PROPERTIES COMPILE_FLAGS ${DLL_VISIBILITY_TWEAK})