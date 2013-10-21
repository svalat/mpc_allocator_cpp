/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <cstddef>
#include "Debug.h"
#include "SmallRunContainer.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
SmallRunContainer::SmallRunContainer ( Size size )
{
	//errors
	//allocAssert(size >= SMALL_RUN_SIZE);

	//init params
	this->size = size;
	this->reservedRuns = 0;

	//split in small run and reg to free list
	setupSplitting();
}

/*******************  FUNCTION  *********************/
SmallChunkRun* SmallRunContainer::getEmptyRun ( void )
{
	//errors
	allocAssert(this != NULL);

	//pop chunk run
	SmallChunkRun * res = emptyRuns.popFirst();

	//update counter
	if (res != NULL)
		this->reservedRuns++;
	
	//check
	allocAssert(reservedRuns <= size / SMALL_RUN_SIZE + 1);
	
	return res;
}

/*******************  FUNCTION  *********************/
SmallRunContainer* SmallRunContainer::getFromListHandler ( ListElement* list )
{
	char * ptr = (char*)list;
	ptr -= offsetof(SmallRunContainer,listHandler);
	SmallRunContainer * res = (SmallRunContainer*)ptr;
	allocAssert(&res->listHandler == list);
	return res;
}

/*******************  FUNCTION  *********************/
ListElement* SmallRunContainer::getListHandler ( void )
{
	return &listHandler;
}

/*******************  FUNCTION  *********************/
bool SmallRunContainer::isEmpty ( void )
{
	return (reservedRuns == 0);
}

/*******************  FUNCTION  *********************/
void SmallRunContainer::regEmpty ( SmallChunkRun* run )
{
	//errors
	allocAssert(run != NULL);
	allocAssert(run->isEmpty());
	allocAssert((void*)run >= (void*)ceilToPowOf2((Addr)this,PAGE_SIZE));
	allocAssert((void*)run < addrOffset((void*)this,size));
	
	if (run != NULL)
	{
		allocAssert(reservedRuns > 0);
		emptyRuns.putFirst(run);
		this->reservedRuns--;
	}
}

/*******************  FUNCTION  *********************/
void SmallRunContainer::setupSplitting ( void )
{
	//setup addresses
	Addr addr = (Addr)(this+1);
	Addr ptrStart = ceilToPowOf2(addr,SMALL_RUN_SIZE);
	Addr ptrEnd = ceilToPowOf2(addr+size,SMALL_RUN_SIZE);
	
	//loop to split
	for (Addr basePtr = ptrStart ; basePtr < ptrEnd ; basePtr+=SMALL_RUN_SIZE)
	{
		size_t skipedSize = 0;
		//calc skip
		if (basePtr < addr)
			skipedSize = addr - basePtr;
		allocAssert(skipedSize < SMALL_RUN_SIZE);

		//create run
		SmallChunkRun * run = new ((void*)basePtr) SmallChunkRun(skipedSize,0,this);
		
		//insert in empty list
		emptyRuns.putFirst(run);
	}
}

/*******************  FUNCTION  *********************/
SmallRunContainer* SmallRunContainer::setup ( void* ptr, Size size )
{
	//errors
	allocAssert(ptr != NULL);
	allocAssert(size >= sizeof(SmallRunContainer));
	
	return new (ptr) SmallRunContainer(size);
}


};
