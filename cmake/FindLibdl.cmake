######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

# Find the dl library
#
#  This module defines the following variables:
#     LIBDL_FOUND       - True if LIBDL_INCLUDE_DIR & LIBDL_LIBRARY are found
#     LIBDL_LIBRARIES   - Set when LIBDL_LIBRARY is found
#     LIBDL_INCLUDE_DIRS - Set when LIBDL_INCLUDE_DIR is found
#
#     LIBDL_INCLUDE_DIR - where to find dlfcn.h, etc.
#     LIBDL_LIBRARY     - the libdl library
#

find_path(LIBDL_INCLUDE_DIR
	NAMES dlfcn.h
	PATHS ${LIBDL_PREFIX}/include ${CMAKE_INSTALL_PREFIX}/include
	DOC "The libdl include directory"
)

find_library(LIBDL_LIBRARY
	NAMES dl
	PATHS ${LIBDL_PREFIX}/lib ${CMAKE_INSTALL_PREFIX}/lib
	DOC "The libdl library"
)

# handle the QUIETLY and REQUIRED arguments and set ShellToolKit_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBDL DEFAULT_MSG LIBDL_LIBRARY LIBDL_INCLUDE_DIR)

if(HWLOC_FOUND)
  set( LIBDL_LIBRARIES ${HWLOC_LIBRARY} )
  set( LIBDL_INCLUDE_DIRS ${HWLOC_INCLUDE_DIR} )
endif()

mark_as_advanced(HWLOC_INCLUDE_DIR HWLOC_LIBRARY)
