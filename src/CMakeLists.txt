
file(WRITE ${CMAKE_BINARY_DIR}/dummy.c "")
add_library(alloc SHARED ${CMAKE_BINARY_DIR}/dummy.c)
target_link_libraries(alloc alloc_mmsource alloc_medium alloc_json alloc_portability alloc_common)
target_link_libraries(alloc ${CMAKE_THREAD_LIBS_INIT})

add_subdirectory(common)
add_subdirectory(json)
add_subdirectory(mmsource)
add_subdirectory(chunks)
add_subdirectory(portability)
add_subdirectory(tests)