
include_directories(${GTEST_INCLUDE_DIRS})
find_package(OpenMP REQUIRED)
find_package(GTest  REQUIRED)
find_package(GMock  REQUIRED)
set(TEST_LIST TestLinkList TestMediumChunk TestSpinlock TestMediumFreePool TestCommon TestMediumAllocator)

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
	add_test(${test} ${test})
ENDFOREACH(test ${TEST_LIST})