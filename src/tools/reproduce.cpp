/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include <PosixAllocator.h>
#include "TraceReader.h"
#include "BugReproducer.h"

/**********************  USING  *********************/
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
void abortHandler(void)
{
	allocDebug("Do abort, convert to Int(42) exception.");
	throw 42;
}

/*******************  FUNCTION  *********************/
int main(int argc,char ** argv)
{
	assert(argc == 2);
	TraceReader reader(argv[1]);
	BugReproducer reproduce;
	
	MPCAllocator::setAbortHandler(abortHandler);
	reproduce.run(reader);
	
	return EXIT_SUCCESS;
}
