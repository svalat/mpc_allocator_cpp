######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
#User options
set(LIB_SUFFIX no CACHE STRING "Define the suffix to append to lib directory name to support lib64 exotic name on some distributions.")
set(ENABLE_JUNIT_OUTPUT no CACHE BOOL "Generate a JUnit XML file for each test while running svUnitTest test suite.")
# set(ENABLE_VALGRIND no CACHE BOOL "Wrap all test execution with valgrind memcheck to generate XML report for each one.")
set(ENABLE_GCC_COVERAGE no CACHE BOOL "Compile htopml library with coverage GCC CFLAGS.")
set(DISABLE_TESTS no CACHE BOOL "Disable compilation of svUnitTest tests.")
set(ENABLE_ONE_TEST no CACHE BOOL "Compile all tests in one executable instead of one per class.")
#set(ENABLE_LOCAL_PATHS no CACHE BOOL "Setup absolute path to run in build directory, not for installation.")
# set(ENABLE_RPATH no CACHE BOOL "Force to use rpath for all libs when installing.")
# set(SVUNITTEST_PREFIX no CACHE STRING "Prefix to find svUnitTest if present.")

######################################################
#Take care of lib suffix for lib64 directory on some distributions
if (NOT LIB_SUFFIX)
	set(LIB_SUFFIX "")
endif (NOT LIB_SUFFIX)

######################################################
#Display info on current configuration
macro (print_variable_status var_name)
	if (${var_name})
		message(STATUS "${var_name} : yes")
	else (${var_value})
		message(STATUS "${var_name} : no")
	endif (${var_name})
endmacro (print_variable_status)

######################################################
#enable coverage profiling
IF (ENABLE_GCC_COVERAGE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fprofile-arcs -ftest-coverage")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -fprofile-arcs -ftest-coverage")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
ENDIF (ENABLE_GCC_COVERAGE)

######################################################
#IF (ENABLE_LOCAL_PATHS)
#	add_definitions(-DHTOPML_EXTDEPS_PATH="${CMAKE_SOURCE_DIR}/extern-deps")
#	add_definitions(-DHTOPML_WWW_PATH="${CMAKE_SOURCE_DIR}/src/www")
#ELSE (ENABLE_LOCAL_PATHS)
#	add_definitions(-DHTOPML_EXTDEPS_PATH="${CMAKE_INSTALL_PREFIX}/share/htopml/www")
#	add_definitions(-DHTOPML_WWW_PATH="${CMAKE_INSTALL_PREFIX}/share/htopml/www")
#ENDIF (ENABLE_LOCAL_PATHS)

######################################################
IF (ENABLE_RPATH)
	SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
	SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
ENDIF(ENABLE_RPATH)

######################################################
IF (OPENPA_FOUND)
	include_directories(${OPENPA_INCLUDE_DIR})
ENDIF (OPENPA_FOUND)

######################################################
#select portability mode
set(PORTABILITY_OS      Linux  )
set(PORTABILITY_LOCKS   Pthread)
set(PORTABILITY_TOPO    Hwloc  )
IF (OPENPA_FOUND)
	set(PORTABILITY_ATOMICS OpenPA )
ELSE (OPENPA_FOUND)
	set(PORTABILITY_ATOMICS GCC    )
ENDIF(OPENPA_FOUND)

#enable features for code
add_definitions(-DOS_${PORTABILITY_OS} -DLOCKS_${PORTABILITY_LOCKS} -DATOMICS_${PORTABILITY_ATOMICS})

######################################################
message(STATUS "-------------------- SUMMARY ----------------------- --")
message(STATUS "PREFIX : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "LIB_SUFFIX : ${LIB_SUFFIX}")
print_variable_status("ENABLE_JUNIT_OUTPUT")
#print_variable_status("ENABLE_VALGRIND")
print_variable_status("ENABLE_GCC_COVERAGE")
print_variable_status("DISABLE_TESTS")
print_variable_status("ENABLE_ONE_TEST")
message(STATUS "PORTABILITY_OS : ${PORTABILITY_OS}")
message(STATUS "PORTABILITY_TOPO : ${PORTABILITY_TOPO}")
message(STATUS "PORTABILITY_LOCKS : ${PORTABILITY_LOCKS}")
message(STATUS "PORTABILITY_ATOMICS : ${PORTABILITY_ATOMICS}")
message(STATUS "CXXFLAGS : ${CMAKE_CXX_FLAGS}")
message(STATUS "---------------------------------------------------- --")
