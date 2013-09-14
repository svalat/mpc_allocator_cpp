/********************  HEADERS  *********************/
#include <stddef.h>
#include "CachedMMSource.h"
#include "Debug.h"
#include <OS.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator 
{
	
/*******************  FUNCTION  *********************/
FreeMacroBloc::FreeMacroBloc ( Size totalSize )
{
	allocAssert(totalSize > sizeof(FreeMacroBloc));
	this->totalSize = totalSize;
}

/*******************  FUNCTION  *********************/
FreeMacroBloc* FreeMacroBloc::setup ( void* ptr, Size totalSize )
{
	return new (ptr) FreeMacroBloc(totalSize);
}

/*******************  FUNCTION  *********************/
Size FreeMacroBloc::getTotalSize ( void ) const
{
	return this->totalSize;
}

/*******************  FUNCTION  *********************/
FreeMacroBloc * FreeMacroBloc::getFromListHandler ( ListElement* list )
{
	return (FreeMacroBloc*)(addrOffset(list,-offsetof(FreeMacroBloc,listElement)));
}

/*******************  FUNCTION  *********************/
ListElement* FreeMacroBloc::getListHandler ( void )
{
	return &listElement;
}

/*******************  FUNCTION  *********************/
FreeMacroBloc * FreeMacroBloc::from ( RegionSegmentHeader* segment )
{
	allocAssert(segment != NULL);
	FreeMacroBloc * res = new (segment)FreeMacroBloc(segment->getTotalSize());
	return res;
}

/*******************  FUNCTION  *********************/
CachedMMSource::CachedMMSource ( RegionRegistry * registry,size_t maxSize, size_t threashold ,bool keepResidut)
{
	this->maxSize = maxSize;
	this->threashold = threashold;
	this->keepResidut = keepResidut;
	this->registry = registry;
	this->currentSize = 0;
}

/*******************  FUNCTION  *********************/
CachedMMSource::~CachedMMSource ( void )
{
	freeAll();
}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* CachedMMSource::map ( size_t innerSize, bool* zeroFilled, IChunkManager* manager )
{
	//vars
	RegionSegmentHeader* res = NULL;

	//errors
	allocAssert(innerSize > 0);
	
	//compute total size
	size_t totalSize = innerSize + sizeof(RegionSegmentHeader);
	
	//if to small
	if (totalSize < REGION_SPLITTING)
		totalSize = REGION_SPLITTING;
	
	//roudn to multiple of page size
	totalSize = upToPowOf2(totalSize,PAGE_SIZE);

	//manage zero status
	bool zero = false;
	/*if (zeroFilled != NULL)
		zero = *zeroFilled;*/

	//search in cache if smaller than threashold
	if (totalSize <= threashold)
	{
		res = searchInCache(totalSize,manager);
		zero = false;
	}
	
	//if not found of too large, do real mmap
	if (res == NULL)
	{
		void * ptr = OS::mmap(NULL,totalSize);
		allocCondWarning(ptr != NULL,"Failed to get memory from OS with mmap, maybe get OOM.");
		zero = true;
		
		//not found
		if (ptr == NULL)
			return NULL;
		else
			res = RegionSegmentHeader::setup(ptr,totalSize,manager);
	}
	
	//register
	if (registry != NULL && manager != NULL && res != NULL)
		registry->setEntry(res);
	
	//setup zero status
	if (zeroFilled != NULL)
		*zeroFilled = zero;

	return res;
}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* CachedMMSource::remap ( RegionSegmentHeader* oldSegment, size_t newInnerSize, IChunkManager* manager )
{
	//errors
	allocAssert(oldSegment != NULL);
	
	//checkup size
	size_t totalSize = newInnerSize + sizeof(RegionSegmentHeader);
	if (totalSize < REGION_SPLITTING)
		totalSize = REGION_SPLITTING;
	totalSize = upToPowOf2(totalSize,PAGE_SIZE);

	//unregister
	if (registry != NULL && oldSegment->getManager() != NULL)
		registry->remove(oldSegment);

	//remap
	void * ptr = OS::mremap(oldSegment,oldSegment->getTotalSize(),totalSize);

	//register
	RegionSegmentHeader *res;
	if (registry != NULL && manager != NULL)
		res = registry->setEntry(ptr,totalSize,manager);
	else
		res = RegionSegmentHeader::setup(ptr,totalSize,manager);
	
	return res;
}

/*******************  FUNCTION  *********************/
void CachedMMSource::unmap ( RegionSegmentHeader* segment )
{
	//errors
	allocAssert(segment != NULL);
	
	//unregister
	if (registry != NULL && segment->getManager() != NULL)
		registry->remove(segment);
	
	//if small, keep, other wise unmap
	size_t size = segment->getTotalSize();
	if (size > threashold || size + currentSize > maxSize)
	{
		OS::munmap(segment,segment->getTotalSize());
	} else {
		START_CRITICAL(spinlock);
			freeList.putFirst(FreeMacroBloc::from(segment));
			currentSize += size;
		END_CRITICAL;
	}
}

/*******************  FUNCTION  *********************/
void CachedMMSource::freeAll ( void )
{
	START_CRITICAL(spinlock);
		//loop on all and unmap
		FreeMacroBlocList::Iterator it = freeList.begin();
		while (it != freeList.end())
		{
			size_t totalSize = it->getTotalSize();
			currentSize -= totalSize;
			void * ptr = &(*it);
			//need to increment before unmap, otherwise segfaut due to access to prev/next
			++it;
			OS::munmap(ptr,totalSize);
		}
	
		//check
		allocAssert(currentSize == 0);
	END_CRITICAL;
}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* CachedMMSource::searchInCache ( size_t totalSize, IChunkManager* manager )
{
	//errors
	allocAssert(totalSize >= REGION_SPLITTING);
	allocAssert(totalSize <= threashold);
	
	//to capture not exact matching
	FreeMacroBloc * best = freeList.getFirst();
	
	//not found 
	if (best == NULL)
		return NULL;
	
	//save best delta
	size_t bestDelta = labs(totalSize - best->getTotalSize());

	//search most adapted
	START_CRITICAL(spinlock);
		for (FreeMacroBlocList::Iterator it = freeList.begin() ; it != freeList.end() ; ++it)
		{
			size_t delta = labs(it->getTotalSize() - totalSize);
			//find better size
			if (delta < bestDelta)
			{
				best = &(*it);
				bestDelta = delta;
			}
			//exact match, no need to search more
			if (delta == 0)
				break;
		}
	
		//update current size
		if (best != NULL)
		{
			FreeMacroBlocList::remove(best);
			currentSize -= best->getTotalSize();
		}
	END_CRITICAL;

	//if to large, split
	if (bestDelta != 0)
		best = fixReuseSize(best,totalSize);

	if (best == NULL)
		return NULL;
	else
		return RegionSegmentHeader::setup(best,best->getTotalSize(),manager);
}

/*******************  FUNCTION  *********************/
FreeMacroBloc* CachedMMSource::fixReuseSize ( FreeMacroBloc* segment, size_t totalSize )
{
	//Vars
	FreeMacroBloc * res = NULL;

	//errors
	allocAssert(segment != NULL);
	
	//extract size
	size_t size = segment->getTotalSize();
	allocAssert(size != totalSize);
	
	//if too small, mremap, otherwise split
	if (size < totalSize)
	{
		void * ptr = OS::mremap((void*)segment,size,totalSize);
		res = FreeMacroBloc::setup(ptr,totalSize);
	} else {
		//split
		res = FreeMacroBloc::setup((void*)segment,totalSize);
		
		//point next
		void * next = addrOffset(res,totalSize);
		size_t nextSize = size - totalSize;

		//keep next for reuse of return to OS
		if (keepResidut && nextSize <= threashold)
		{
			START_CRITICAL(spinlock);
				FreeMacroBloc * residut = FreeMacroBloc::setup(next,nextSize);
				freeList.putFirst(residut);
				currentSize += residut->getTotalSize();
			END_CRITICAL;
		} else {
			OS::munmap(next,nextSize);
		}
	}
	
	return res;
}

};
