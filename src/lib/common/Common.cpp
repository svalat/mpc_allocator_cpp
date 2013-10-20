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
