
file(WRITE ${CMAKE_BINARY_DIR}/dummy.c "")
add_library(alloc SHARED ${CMAKE_BINARY_DIR}/dummy.c 
	$<TARGET_OBJECTS:alloc_mmsource> 
	$<TARGET_OBJECTS:alloc_medium> 
	$<TARGET_OBJECTS:alloc_json> 
	$<TARGET_OBJECTS:alloc_common> 
	$<TARGET_OBJECTS:alloc_portability>)
target_link_libraries(alloc ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(common)
add_subdirectory(json)
add_subdirectory(mmsource)
add_subdirectory(chunks)
add_subdirectory(portability)
add_subdirectory(tests)