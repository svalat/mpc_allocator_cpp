/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include <cassert>
#include <PosixAllocator.h>
#include <map>
#include <cstring>
#include "TraceReader.h"
#include "CollisionChecker.h"
#include "EventDispatchReplay.h"

/**********************  USING  *********************/
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
int main(int argc,char ** argv)
{
	assert(argc == 2);
	TraceReader reader(argv[1]);
	EventDispatchReplay replay;
	
	replay.run(reader);
	
	return EXIT_SUCCESS;
}
