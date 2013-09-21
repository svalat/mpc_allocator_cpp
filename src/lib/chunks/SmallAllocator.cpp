/********************  HEADERS  *********************/
#include <Debug.h>
#include <cstring>
#include <cstddef>
#include "SmallAllocator.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CONSTS  *********************/
static const size_t SMALL_SIZE_CLASSES[NB_SIZE_CLASS] = {8, 16, 24, 32, 48, 64, 80, 96, 112, 128};

/*******************  FUNCTION  *********************/
SmallAllocator::SmallAllocator ( bool useLocks, IMMSource* memorySource )
{
	this->useLocks = useLocks;
	this->memorySource = memorySource;
	for (int i = 0 ; i < NB_SIZE_CLASS ; i++)
		activRuns[i] = NULL;
}

/*******************  FUNCTION  *********************/
void* SmallAllocator::malloc ( size_t size, size_t align, bool* zeroFilled )
{
	//vars
	void * res = NULL;

	//align is not supported up to now
	allocAssume(align == sizeof(void*), "TODO : support align");
	
	//get related size class
	int sizeClass = getSizeClass(size);
	allocAssume(sizeClass >= 0,"Invalid size, not candidate to use small chunk mechnism.");
	allocAssert(SMALL_SIZE_CLASSES[sizeClass] % align == 0);
	
	//take lock for the current function
	OPTIONAL_CRITICAL(spinlock,useLocks);
		//get active run
		SmallChunkRun * run = getActivRunForSize(sizeClass);
		
		//try to allocate with current run
		if (run != NULL)
			res = run->malloc(size,align,zeroFilled);
		
		//if not found, need to rotate current runs and retry
		if (res == NULL)
		{
			run = updateActivRunForSize(sizeClass);
			if (run != NULL)
				res = run->malloc(size,align,zeroFilled);
		}
	END_CRITICAL
	
	if (zeroFilled != NULL)
		*zeroFilled = false;
	
	//return
	return res;
}

/*******************  FUNCTION  *********************/
void SmallAllocator::fill ( void* ptr, size_t size, RegionRegistry* registry, bool lock )
{
	//cast to ease usage
	void * addr = ptr;
	
	//errors
	allocAssert(ptr != NULL);

	//if need register, create macro bloc
	if (registry != NULL)
	{
		RegionSegmentHeader * segment = registry->setEntry(ptr,size,this);
		addr = segment->getPtr();
		size = segment->getInnerSize();
	}
	
	//setup run container
	SmallRunContainer * container = SmallRunContainer::setup(addr,size);
	allocAssert(container != NULL);

	//reg in list
	OPTIONAL_CRITICAL(spinlock,useLocks && lock);
		containers.putLast(container);
	END_CRITICAL
}

/*******************  FUNCTION  *********************/
void SmallAllocator::fill ( void* ptr, size_t size, RegionRegistry* registry )
{
	fill(ptr,size,registry,true);
}

/*******************  FUNCTION  *********************/
void SmallAllocator::refill ( void )
{
	//trivial
	if (memorySource == NULL)
		return;
	
	//request mem
	RegionSegmentHeader * segment = memorySource->map(REGION_SPLITTING-sizeof(RegionSegmentHeader),NULL,this);
	if (segment == NULL)
		return;
	allocAssert(segment->getTotalSize() == REGION_SPLITTING);
	
	//get inner segment
	void * ptr = segment->getPtr();
	allocAssert(segment == RegionSegmentHeader::getSegment(ptr));
	
	//build chunk
	Size innerSize = segment->getInnerSize();
	fill(ptr,innerSize,NULL,false);
}

/*******************  FUNCTION  *********************/
void SmallAllocator::free ( void* ptr )
{
	//trivial
	if (ptr == NULL)
		return;
	
	//find small chunk
	SmallChunkRun * run = getRun(ptr);
	allocAssert(run != NULL);
	//MAYBE put warning
	if (run == NULL)
		return;
	
	OPTIONAL_CRITICAL(spinlock,useLocks);
		//free
		run->free(ptr);
		
		//if empty move to empty list
		if (run->isEmpty())
			markRunAsFree(run);
	END_CRITICAL;
}

/*******************  FUNCTION  *********************/
void SmallAllocator::markRunAsFree ( SmallChunkRun* run )
{
	//errors
	allocAssert(run != NULL);
	allocAssert(run->isEmpty());
	
	//reg empty
	SmallRunContainer * container = run->getContainer();
	allocAssert(container != NULL);
	
	//check current usage
	int sizeClass = getSizeClass(run->getSplitting());
	allocAssert(sizeClass >= 0);
	if (activRuns[sizeClass] == run)
	{
		activRuns[sizeClass] = NULL;
	} else {
		SmallChunkRunList::remove(run);
	}
	
	//register as free
	run->setSplitting(0);
	container->regEmpty(run);
	
	//if container is empty, remove it
	if (container->isEmpty() && memorySource != NULL)
	{
		containers.remove(container);
		memorySource->unmap(RegionSegmentHeader::getSegment(container));
	}
}

/*******************  FUNCTION  *********************/
SmallChunkRun* SmallAllocator::getActivRunForSize ( int sizeClass )
{
	allocAssert(sizeClass >= 0 && sizeClass < NB_SIZE_CLASS);
	SmallChunkRun * res = activRuns[sizeClass];
	return res;
}

/*******************  FUNCTION  *********************/
size_t SmallAllocator::getInnerSize ( void* ptr )
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
size_t SmallAllocator::getRequestedSize ( void* ptr )
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
size_t SmallAllocator::getTotalSize ( void* ptr )
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
SmallChunkRun* SmallAllocator::getRun ( void* ptr )
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
//8, 16, 24, 32, 48, 64, 80, 96, 128
int SmallAllocator::getSizeClass ( size_t size ) const
{
	//errors
	allocAssert(sizeof(SMALL_SIZE_CLASSES) / sizeof(size_t) == NB_SIZE_CLASS);
	allocAssert(size <= SMALL_CHUNK_MAX_SIZE);
	allocAssert(size > 0);

	//trivial
	if (size > SMALL_CHUNK_MAX_SIZE)
		return -1;
	
	//if too small
	if (size < 8)
		size = 8;
	
	//calc from 8 to 32
	int res;
	if (size <= 32)
		res = (size - 1) / 8;
	else
		res = (size - 1) / 16 + 2;
	
	allocAssert(SMALL_SIZE_CLASSES[res] >= size);
	if (res > 0)
		allocAssert(SMALL_SIZE_CLASSES[res-1] < size);	
	
	return res;
}

/*******************  FUNCTION  *********************/
//TODO maybe we can avoid to take the lock 2 times
void* SmallAllocator::realloc ( void* ptr, size_t size )
{
	//trivial cases
	if (ptr == NULL)
	{
		return this->malloc(size);
	} else if (size == 0) {
		this->free(ptr);
		return NULL;
	}
	
	//get size classes
	SmallChunkRun * oldRun = getRun(ptr);
	allocAssume(oldRun != NULL,"Invalid old pointer for realloc on SmallAllocator.");
	int oldClass = getSizeClass(oldRun->getSplitting());
	int newClass = getSizeClass(size);
	allocAssume(newClass >= 0,"Invalid new size for realloc on SmallAllocator.");
	
	//if same class, do nothing, otherwise to realloc
	if (newClass == oldClass)
		return ptr;
	
	//alloc, copy, free
	void * res = this->malloc(size);
	if (res != NULL)
		memcpy(res,ptr,min(size,(Size)oldRun->getSplitting()));
	this->free(ptr);
	
	//ok return the segment
	return res;
}

/*******************  FUNCTION  *********************/
SmallChunkRun* SmallAllocator::findEmptyRun ( void )
{
	SmallChunkRun * res = NULL;
	
	//search in containers
	for (SmallRunContainerList::Iterator it = containers.begin() ; it != containers.end() ; ++it)
	{
		res = it->getEmptyRun();
		if (res != NULL)
			break;
	}
	
	return res;
}

/*******************  FUNCTION  *********************/
SmallChunkRun* SmallAllocator::updateActivRunForSize ( int sizeClass )
{
	//errors
	allocAssert(sizeClass >= 0 && sizeClass < NB_SIZE_CLASS);
	if (activRuns[sizeClass] != NULL)
		allocAssert(activRuns[sizeClass]->isFull());
	
	//search in list
	SmallChunkRun * run = NULL;
	for (SmallChunkRunList::Iterator it = inUse[sizeClass].begin() ; it != inUse[sizeClass].end() ; ++it)
	{
		if (it->isFull() == false)
		{
			run = &(*it);
			break;
		}
	}
	if (run != NULL)
		inUse[sizeClass].remove(run);
	
	//if have not, try in empty list
	if (run == NULL)
	{
		run = findEmptyRun();
		//need to refill
		if (run == NULL)
		{
			refill();
			run = findEmptyRun();
		}
		//setup splitting in run
		if (run != NULL)
			run->setSplitting(SMALL_SIZE_CLASSES[sizeClass]);
	}

	//if have one
	if (run != NULL)
	{
		//insert in FIFO
		if (activRuns[sizeClass] != NULL)
			inUse[sizeClass].putLast(activRuns[sizeClass]);
		activRuns[sizeClass] = run;
	}

	//return it
	return run;
}

};
