include(${CMAKE_SOURCE_DIR}/cmake/unittest-macros.cmake)
include_directories(${GTEST_INCLUDE_DIRS})
set(TEST_LIST TestLinkList TestMediumChunk TestSpinlock TestMediumFreePool TestCommon TestMediumAllocator TestRegionRegistry TestOS TestDummyMMSource)

include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

if(OPENMP_FOUND)
	message("OPENMP FOUND")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -DHAVE_OPENMP")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -DHAVE_OPENMP")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

FOREACH(test ${TEST_LIST})
	add_executable(${test} ${test}.cpp)
	target_link_libraries(${test} alloc ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})
	cmr_add_test(${test} ${test})
ENDFOREACH(test ${TEST_LIST})