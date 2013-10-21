/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include "Common.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{
	
/*******************  FUNCTION  *********************/
static AbortHandler gblAbortHandler = ::abort;
	
/*******************  FUNCTION  *********************/
AbortHandler setAbortHandler ( AbortHandler handler )
{
	AbortHandler oldHandler = gblAbortHandler;
	gblAbortHandler = handler;
	return oldHandler;
}

/*******************  FUNCTION  *********************/
void abort ( void )
{
	gblAbortHandler();
}

};
