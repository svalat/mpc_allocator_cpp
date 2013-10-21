######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
#Enable fpic, if old cmake use the manual def, otherwise let cmake do the right thing
MACRO(my_enable_fpic)
	IF   (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
		set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	ELSE (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
		set(CMAKE_CXX_FLAGS ${CMAKE_CXX_CLAGS} -fPIC)
		set(CMAKE_C_FLAGS ${CMAKE_CXX_CLAGS} -fPIC)
	ENDIF(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.7)
ENDMACRO(my_enable_fpic)

######################################################
#To quicly print variable status
MACRO (print_variable_status var_name)
	if (${var_name})
		message(STATUS "${var_name} : yes")
	else (${var_value})
		message(STATUS "${var_name} : no")
	endif (${var_name})
ENDMACRO (print_variable_status)

######################################################
MACRO(setup_internal_gmock_and_gtest)
	add_subdirectory(${CMAKE_SOURCE_DIR}/extern/gmock-1.7.0)
	set(GMOCK_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern/gmock-1.7.0/include)
	set(GMOCK_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/extern/gmock-1.7.0/include)
	set(GMOCK_BOTH_LIBRARIES gmock gmock_main)
	set(GTEST_BOTH_LIBRARIES gtest)
	set(GTEST_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern/gmock-1.7.0/gtest/include/)
	set(GTEST_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/extern/gmock-1.7.0/gtest/include/)
ENDMACRO(setup_internal_gmock_and_gtest)
