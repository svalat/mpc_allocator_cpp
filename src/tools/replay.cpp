/********************  HEADERS  *********************/
#include <cstdio>
#include <cassert>
#include <PosixAllocator.h>
#include <map>
#include <cstring>
#include "TraceReader.h"
#include "CollisionChecker.h"
#include "EventDispatchReplay.h"

//make it silent
#define trace_printf(...) do{} while(0)

/**********************  USING  *********************/
using namespace std;
using namespace MPCAllocator;

/*******************  FUNCTION  *********************/
void checkIsZero(void * ptr,size_t size)
{
	for (size_t i = 0 ; i < size ; i++)
	{
		allocAssume(((char*)ptr)[i] == 0,"Required zero but get non initialized memory.");
	}
};

/*******************  FUNCTION  *********************/
int main(int argc,char ** argv)
{
	assert(argc == 2);
	TraceReader reader(argv[1]);
	TraceEntryComplete entry;
	EventDispatchReplay replay;
	
	while (reader.readNext(entry))
	{
		replay.run(entry);
	}
	
	return EXIT_SUCCESS;
}
