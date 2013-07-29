######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
add_library(alloc SHARED $<TARGET_OBJECTS:alloc_mmsource> 
                         $<TARGET_OBJECTS:alloc_medium>
                         $<TARGET_OBJECTS:alloc_json> 
                         $<TARGET_OBJECTS:alloc_common> 
                         $<TARGET_OBJECTS:alloc_portability>)
target_link_libraries(alloc ${CMAKE_THREAD_LIBS_INIT})

######################################################
add_subdirectory(common)
add_subdirectory(json)
add_subdirectory(mmsource)
add_subdirectory(chunks)
add_subdirectory(portability)

######################################################
if (NOT DISABLE_TESTS)
	add_subdirectory(tests)
endif (NOT DISABLE_TESTS)
