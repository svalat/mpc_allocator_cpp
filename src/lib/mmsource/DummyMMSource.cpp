/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include "Common.h"
#include "RegionRegistry.h"
#include "OS.h"
#include "Debug.h"
#include "DummyMMSource.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
DummyMMSource::DummyMMSource ( RegionRegistry* registry)
{
	this->registry = registry;
}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* DummyMMSource::map ( size_t innerSize, bool* zeroFilled, IChunkManager * manager )
{
	//errors
	allocAssert(innerSize > 0);
	
	//compute total size
	size_t totalSize = innerSize + sizeof(RegionSegmentHeader);
	
	//TODO warning if really too small
	
	//if to small
	if (totalSize < REGION_SPLITTING)
		totalSize = REGION_SPLITTING;
	
	//roudn to multiple of page size
	totalSize = upToPowOf2(totalSize,PAGE_SIZE);
	
	//allocate
	void * ptr = OS::mmap(NULL,totalSize);

	//register
	RegionSegmentHeader *res;
	if (registry != NULL && manager != NULL)
		res = registry->setEntry(ptr,totalSize,manager);
	else
		res = RegionSegmentHeader::setup(ptr,totalSize,manager);
	
	//zeroFilled
	if (zeroFilled != NULL)
		*zeroFilled = true;
	
	return res;
}

/*******************  FUNCTION  *********************/
void DummyMMSource::unmap ( RegionSegmentHeader* segment )
{
	//errors
	allocAssert(segment != NULL);
	
	//unregister
	if (registry != NULL && segment->getManager() != NULL)
		registry->remove(segment);
	
	//unmap it
	OS::munmap(segment,segment->getTotalSize());
}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* DummyMMSource::remap ( RegionSegmentHeader* oldSegment, size_t newInnerSize , IChunkManager * manager )
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

};
