######################################################
#            PROJECT  : MPC_Allocator_CPP            #
#            VERSION  : 0.0.0                        #
#            DATE     : 07/2013                      #
#            AUTHOR   : Valat Sébastien              #
#            LICENSE  : CeCILL-C                     #
######################################################

######################################################
#For allocator
find_package(Threads REQUIRED)
find_package(OpenPA  REQUIRED)
#For tests
if (NOT DISABLE_TESTS)
	find_package(OpenMP  REQUIRED)
	find_package(GTest   REQUIRED)
	find_package(GMock   REQUIRED)
endif (NOT DISABLE_TESTS)
