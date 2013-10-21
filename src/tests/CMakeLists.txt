######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
include(${CMAKE_SOURCE_DIR}/cmake/unittest-macros.cmake)

######################################################
set(TEST_LIST TestLinkList TestMediumChunk TestSpinlock TestMediumFreePool 
	TestCommon TestMediumAllocator TestRegionRegistry TestOS TestDummyMMSource 
	TestAtomicPtr TestSmallChunk TestSmallAllocator TestPosixAllocator 
	TestPaddedChunk TestCachedMMSource TestSmallRunContainer)

######################################################
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

######################################################
if(OPENMP_FOUND)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -DHAVE_OPENMP")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -DHAVE_OPENMP")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

######################################################
if (ENABLE_ONE_TEST)
	set(TEST_SOURCES)
	FOREACH(test ${TEST_LIST})
		LIST(APPEND TEST_SOURCES ${test}.cpp)
	ENDFOREACH(test ${TEST_LIST})
	add_executable(tests ${TEST_SOURCES})
	target_link_libraries(tests alloc_base ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})
	cmr_add_test(tests tests)
else (ENABLE_ONE_TEST)
	FOREACH(test ${TEST_LIST})
		add_executable(${test} ${test}.cpp)
		target_link_libraries(${test} alloc_base ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})
		cmr_add_test(${test} ${test})
	ENDFOREACH(test ${TEST_LIST})
endif (ENABLE_ONE_TEST)

######################################################
add_executable(TestMPSCFQueue TestMPSCFQueue.cpp)
target_link_libraries(TestMPSCFQueue alloc_base ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})
add_test(TestMPSCFQueue TestMPSCFQueue valid)
