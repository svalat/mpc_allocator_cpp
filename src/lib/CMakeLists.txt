######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
set(ALLOC_BASE_OBJS $<TARGET_OBJECTS:alloc_mmsource> 
                    $<TARGET_OBJECTS:alloc_medium>
                    $<TARGET_OBJECTS:alloc_json> 
                    $<TARGET_OBJECTS:alloc_common> 
                    $<TARGET_OBJECTS:alloc_portability>
                    $<TARGET_OBJECTS:alloc_posix>)

######################################################
set(ALLOC_BASE_INT_OBJS $<TARGET_OBJECTS:alloc_mmsource_internal> 
                        $<TARGET_OBJECTS:alloc_medium_internal>
                        $<TARGET_OBJECTS:alloc_json_internal> 
                        $<TARGET_OBJECTS:alloc_common_internal> 
                        $<TARGET_OBJECTS:alloc_portability_internal>
                        $<TARGET_OBJECTS:alloc_posix_internal>)

######################################################
add_library(mpc_alloc_base_internal SHARED ${ALLOC_BASE_INT_OBJS})
target_link_libraries(mpc_alloc_base_internal ${CMAKE_THREAD_LIBS_INIT} ${HWLOC_LIBRARY} ${LIBDL_LIBRARY})

######################################################
add_library(mpc_alloc_base SHARED ${ALLOC_BASE_OBJS})
target_link_libraries(mpc_alloc_base ${CMAKE_THREAD_LIBS_INIT} ${HWLOC_LIBRARY} ${LIBDL_LIBRARY})
set_target_properties(mpc_alloc_base PROPERTIES VERSION ${ALLOC_VERSION} SOVERSION ${ALLOC_API_VERSION})

######################################################
add_library(mpc_alloc SHARED ${ALLOC_BASE_OBJS} $<TARGET_OBJECTS:alloc_posix_export>)
target_link_libraries(mpc_alloc ${CMAKE_THREAD_LIBS_INIT} ${HWLOC_LIBRARY} ${LIBDL_LIBRARY})
set_target_properties(mpc_alloc PROPERTIES VERSION ${ALLOC_VERSION} SOVERSION ${ALLOC_API_VERSION})

######################################################
add_subdirectory(chunks)
add_subdirectory(common)
add_subdirectory(json)
add_subdirectory(mmsource)
add_subdirectory(portability)
add_subdirectory(posix)

######################################################
install(TARGETS mpc_alloc mpc_alloc_base DESTINATION lib)
install(FILES mpc_allocator.h DESTINATION include)
