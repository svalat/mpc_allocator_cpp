######################################################
set(ALLOC_BASE_OBJS $<TARGET_OBJECTS:alloc_mmsource> 
                    $<TARGET_OBJECTS:alloc_medium>
                    $<TARGET_OBJECTS:alloc_json> 
                    $<TARGET_OBJECTS:alloc_common> 
                    $<TARGET_OBJECTS:alloc_portability>
                    $<TARGET_OBJECTS:alloc_posix>)

######################################################
add_library(alloc_base SHARED ${ALLOC_BASE_OBJS})
target_link_libraries(alloc_base ${CMAKE_THREAD_LIBS_INIT} ${HWLOC_LIBRARY})

######################################################
add_library(alloc SHARED ${ALLOC_BASE_OBJS} $<TARGET_OBJECTS:alloc_posix_export>)
target_link_libraries(alloc ${CMAKE_THREAD_LIBS_INIT} ${HWLOC_LIBRARY})

######################################################
add_subdirectory(chunks)
add_subdirectory(common)
add_subdirectory(json)
add_subdirectory(mmsource)
add_subdirectory(portability)
add_subdirectory(posix)
