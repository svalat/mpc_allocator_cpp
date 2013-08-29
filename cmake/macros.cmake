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
	IF   (${CMAKE_MAJOR_VERSION} VERSION_GREATER 2.8.8)
		set(CMAKE_POSITION_INDEPENDENT_CODE ON)
	ELSE (${CMAKE_MAJOR_VERSION} VERSION_GREATER 2.8.8)
		set(CMAKE_CXX_CLAGS ${CMAKE_CXX_CLAGS} -fPIC)
		set(CMAKE_C_CLAGS ${CMAKE_CXX_CLAGS} -fPIC)
	ENDIF(${CMAKE_MAJOR_VERSION} VERSION_GREATER 2.8.8)
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
