/********************  HEADERS  *********************/
#include <Debug.h>
#include "SmallChunkAllocator.h"

/*********************  CONSTS  *********************/
static const size_t SMALL_SIZE_CLASSES[NB_SIZE_CLASS] = {8, 16, 24, 32, 48, 64, 80, 96, 128};

/*******************  FUNCTION  *********************/
SmallChunkAllocator::SmallChunkAllocator ( bool useLocks, IMMSource* memorySource )
{
	this->useLocks = useLocks;
	this->memorySource = memorySource;
	for (int i = 0 ; i < NB_SIZE_CLASS ; i++)
		activRuns[i] = NULL;
}

/*******************  FUNCTION  *********************/
void* SmallChunkAllocator::malloc ( size_t size, size_t align, bool* zeroFilled )
{
	//vars
	void * res = NULL;

	//align is not supported up to now
	allocAssume(align == 0, "TODO : support align");
	
	//get related size class
	int sizeClass = getSizeClass(size);
	allocAssume(sizeClass > 0,"Invalid size, not candidate to use small chunk mechnism.");
	
	//take lock for the current function
	OPTIONAL_CRITICAL(spinlock,useLocks);
		//get active run
		SmallChunkRun * run = getActivRunForSize(sizeClass);
		if (run == NULL)
			return NULL;
		
		//try to allocate with current run
		res = run->malloc(size,align,zeroFilled);
		
		//if not found, need to rotate current runs and retry
		if (res == NULL)
		{
			run = updateActivRunForSize(sizeClass);
			res = run->malloc(size,align,zeroFilled);
		}
	END_CRITICAL
	
	//return
	return res;
}

/*******************  FUNCTION  *********************/
void SmallChunkAllocator::fill ( void* ptr, size_t size, RegionRegistry* registry, bool lock )
{
	//cast to ease usage
	Addr addr = (Addr)ptr;
	
	//errors
	allocAssert(ptr != NULL);
	allocAssert(size >= SMALL_RUN_SIZE);

	//if need register, create macro bloc
	if (registry != NULL)
	{
		RegionSegmentHeader * segment = registry->setEntry(ptr,size,this);
		ptr = segment->getPtr();
		size = segment->getInnerSize();
	}

	//calc starting point
	Addr ptrStart = ceilToPowOf2(addr,SMALL_RUN_SIZE);
	Addr ptrEnd = ceilToPowOf2(addr+size,SMALL_RUN_SIZE);
	
	//create all runs
	OPTIONAL_CRITICAL(spinlock,useLocks && lock);
		for (Addr basePtr = ptrStart ; basePtr < ptrEnd ; basePtr++)
		{
			size_t skipedSize = 0;
			//calc skip
			if (basePtr < (Addr)ptr)
				skipedSize = ptrStart - addr;
			allocAssert(skipedSize < SMALL_RUN_SIZE);

			//create run
			SmallChunkRun * run = new ((void*)basePtr) SmallChunkRun(skipedSize);
			
			//insert in empty list
			empty.putFirst(run);
		}
	END_CRITICAL
}

/*******************  FUNCTION  *********************/
void SmallChunkAllocator::fill ( void* ptr, size_t size, RegionRegistry* registry )
{
	fill(ptr,size,registry,true);
}

/*******************  FUNCTION  *********************/
void SmallChunkAllocator::refill ( void )
{
	//trivial
	if (memorySource == NULL)
		return;
	
	//request mem
	RegionSegmentHeader * segment = memorySource->map(REGION_SPLITTING,NULL,this);
	if (segment == NULL)
		return;
	allocAssert(segment->getTotalSize() == REGION_SPLITTING);
	allocAssert(((Addr)segment + segment->getTotalSize()) % SMALL_RUN_SIZE == 0);
	
	//get inner segment
	void * ptr = segment->getPtr();
	assert(segment == RegionSegmentHeader::getSegment(ptr));
	
	//build chunk
	Size innerSize = segment->getInnerSize();
	fill(ptr,innerSize,NULL,false);
}

/*******************  FUNCTION  *********************/
void SmallChunkAllocator::free ( void* ptr )
{
	//trivial
	if (ptr == NULL)
		return;
	
	//find small chunk
	SmallChunkRun * run = getRun(ptr);
	allocAssert(run != NULL);
	//MAYBE put warning
	if (run != NULL)
		return;
	
	//free
	run->free(ptr);
	
	//if empty move to empty list
	if (run->isEmpty())
	{
		//TODO check if it's not the activ one otherwise we need to skip this action
		//TODO improve memory free by pointing a free list per macro bloc, so get more chance
		//to fee one
		SmallChunkRunList::remove(run);
		empty.putFirst(run);
	}
}

/*******************  FUNCTION  *********************/
SmallChunkRun* SmallChunkAllocator::getActivRunForSize ( int sizeClass )
{
	allocAssert(sizeClass >= 0 && sizeClass < NB_SIZE_CLASS);
	SmallChunkRun * res = activRuns[sizeClass];
	if (res == NULL)
		return updateActivRunForSize(sizeClass);
}

/*******************  FUNCTION  *********************/
size_t SmallChunkAllocator::getInnerSize ( void* ptr )
{
	//get the run to request the size
	SmallChunkRun * run = getRun(ptr);
	allocAssert(run != NULL);
	if (run == NULL)
		return 0;

	//return the size
	return run->getInnerSize(ptr);
}

/*******************  FUNCTION  *********************/
size_t SmallChunkAllocator::getRequestedSize ( void* ptr )
{
	//get the run to request the size
	SmallChunkRun * run = getRun(ptr);
	allocAssert(run != NULL);
	if (run == NULL)
		return 0;

	//return the size
	return run->getRequestedSize(ptr);
}

/*******************  FUNCTION  *********************/
size_t SmallChunkAllocator::getTotalSize ( void* ptr )
{
	//get the run to request the size
	SmallChunkRun * run = getRun(ptr);
	allocAssert(run != NULL);
	if (run == NULL)
		return 0;

	//return the size
	return run->getTotalSize(ptr);
}

/*******************  FUNCTION  *********************/
SmallChunkRun* SmallChunkAllocator::getRun ( void* ptr )
{
	//trivia
	if (ptr == NULL)
		return NULL;
	
	//round add
	SmallChunkRun * run = (SmallChunkRun*)ceilToPowOf2((Addr)ptr,SMALL_RUN_SIZE);
	
	//check 
	allocAssert(run->contain(ptr));
	if (run->contain(ptr) == false)
		return NULL;

	return run;
}

/*******************  FUNCTION  *********************/
int SmallChunkAllocator::getSizeClass ( size_t size ) const
{
	//errors
	allocAssert(sizeof(SMALL_SIZE_CLASSES) / sizeof(size_t) == NB_SIZE_CLASS);
	allocAssert(size > SMALL_CHUNK_MAX_SIZE);
	allocAssert(size > 0);

	//trivial
	if (size > SMALL_CHUNK_MAX_SIZE)
		return -1;
	
	//calc from 8 to 32
	int res;
	if (size <= 32)
		res = (size - 1) / 8;
	else
		res = (size - 1) / 16 + 4;
	
	allocAssert(SMALL_SIZE_CLASSES[res] >= size);
	if (res > 0)
		allocAssert(SMALL_SIZE_CLASSES[res-1] < size);	
	
	return res;
}

/*******************  FUNCTION  *********************/
void* SmallChunkAllocator::realloc ( void* ptr, size_t size )
{
#warning TODO
}

/*******************  FUNCTION  *********************/
SmallChunkRun* SmallChunkAllocator::updateActivRunForSize ( int sizeClass )
{
#warning TODO
}

/*******************  FUNCTION  *********************/
