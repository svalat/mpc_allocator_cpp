/********************  HEADERS  *********************/
#include <cstdio>
#include <cassert>
#include "TraceReader.h"
#include "EventDispatchPrint.h"

/**********************  USING  *********************/
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
int main(int argc,char ** argv)
{
	assert(argc == 2);
	TraceReader reader(argv[1]);
	TraceEntryComplete entry;
	EventDispatchPrint printer;
	
	printer.run(reader);
	
	return EXIT_SUCCESS;
}
