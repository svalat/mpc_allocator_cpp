/********************  HEADERS  *********************/
#include "CachedMMSource.h"
#include "Debug.h"
#include <OS.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator 
{

/*******************  FUNCTION  *********************/
CachedMMSource::CachedMMSource ( RegionRegistry * registry,size_t maxSize, size_t threashold ,bool keepResidut)
{
	this->maxSize = maxSize;
	this->threashold = threashold;
	this->keepResidut = keepResidut;
	this->registry = registry;
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
		res = searchInCache(totalSize);
		res->setManager(manager);
		zero = false;
	}
	
	//if not found of too large, do real mmap
	if (res == NULL)
	{
		void * ptr = OS::mmap(NULL,totalSize);
		allocCondWarning(res != NULL,"Failed to get memory from OS with mmap, maybe get OOM.");
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
		segment->setManager(NULL);
		freeList.putFirst(segment);
		currentSize += size;
	}
}

/*******************  FUNCTION  *********************/
void CachedMMSource::freeAll ( void )
{
	//loop on all and unmap
	for (FreeMacroBlocList::Iterator it = freeList.begin() ; it != freeList.end() ; ++it)
	{
		size_t totalSize = it->getTotalSize();
		currentSize -= totalSize;
		OS::munmap(&(*it),totalSize);
	}
	
	//check
	allocAssert(currentSize == 0);
}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* CachedMMSource::searchInCache ( size_t totalSize )
{
	//errors
	allocAssert(totalSize >= REGION_SPLITTING);
	allocAssert(totalSize <= threashold);
	
	//to capture not exact matching
	RegionSegmentHeader * best = freeList.getFirst();
	size_t bestDelta = labs(totalSize - best->getTotalSize());

	//search most adapted
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
		currentSize -= best->getTotalSize();
	
	//if to large, split
	if (bestDelta != 0)
		best = fixReuseSize(best,totalSize);

	return best;
}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* CachedMMSource::fixReuseSize ( RegionSegmentHeader* segment, size_t totalSize )
{
	//Vars
	RegionSegmentHeader * res = NULL;

	//errors
	allocAssert(segment != NULL);
	
	//extract size
	size_t size = segment->getTotalSize();
	allocAssert(size != totalSize);
	
	//if too small, mremap, otherwise split
	if (size < totalSize)
	{
		void * ptr = OS::mremap((void*)segment,size,totalSize);
		res = RegionSegmentHeader::setup(ptr,totalSize,NULL);
	} else {
		//split
		res = RegionSegmentHeader::setup((void*)segment,totalSize,NULL);
		
		//point next
		void * next = addrOffset(res,totalSize);
		size_t nextSize = size - totalSize;

		//keep next for reuse of return to OS
		if (keepResidut)
		{
			RegionSegmentHeader * residut = RegionSegmentHeader::setup(next,nextSize,NULL);
			freeList.putFirst(residut);
			currentSize += residut->getTotalSize();
		} else {
			OS::munmap(next,nextSize);
		}
	}
	
	return res;
}

};
