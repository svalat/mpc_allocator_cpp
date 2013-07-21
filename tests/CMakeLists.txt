include_directories(${GTEST_INCLUDE_DIRS})
set(TEST_LIST TestLinkList TestMediumChunk)

FOREACH(test ${TEST_LIST})
	add_executable(${test} ${test}.cpp)
	target_link_libraries(${test} alloc ${GTEST_BOTH_LIBRARIES})
ENDFOREACH(test ${TEST_LIST})