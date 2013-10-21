/********************  HEADERS  *********************/
#include <cstring>
#include "Debug.h"
#include "RegionRegistry.h"
#include "OS.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
RegionSegmentHeader* RegionSegmentHeader::setup ( void* ptr, Size totalSize, IChunkManager* manager )
{
	//TODO maybe some assume
	allocAssert(ptr != NULL);
	allocAssert( totalSize >= REGION_SPLITTING);
	allocAssert(manager != NULL);
	allocAssert(totalSize >= sizeof(RegionSegmentHeader));

	RegionSegmentHeader * res = (RegionSegmentHeader*)ptr;
	res->totalSize = totalSize;
	res->manager = manager;
	
	return res;
}

/*******************  FUNCTION  *********************/
bool RegionSegmentHeader::contain ( void* ptr ) const
{
	return (ptr >= this && ptr < addrOffset(this,totalSize));
}

/*******************  FUNCTION  *********************/
IChunkManager* RegionSegmentHeader::getManager ( void )
{
	allocAssert(this != NULL);
	return manager;
}

/*******************  FUNCTION  *********************/
Size RegionSegmentHeader::getTotalSize ( void ) const
{
	allocAssert(this != NULL);
	return totalSize;
}

/*******************  FUNCTION  *********************/
Size RegionSegmentHeader::getInnerSize ( void ) const
{
	allocAssert(this != NULL);
	return totalSize - sizeof(*this);
}

/*******************  FUNCTION  *********************/
void* RegionSegmentHeader::getPtr ( void )
{
	if (this == NULL)
		return NULL;
	else
		return this+1;
}

/*******************  FUNCTION  *********************/
bool Region::isEmpty ( void ) const
{
	//TODO optimize by checking page presence
	for (Size i = 0 ; i < REGION_ENTRIES ; i++)
		if (entries[i] != NULL)
			return false;

	return true;
}

/*******************  FUNCTION  *********************/
int RegionRegistry::getRegionId ( Addr addr )
{
	//errors
	allocAssert(addr != ADDR_NULL);
	allocAssert(addr < PHYS_MAX_ADDR);
	
	/** @TODO can be optimize if we consider power of 2 **/
	int id = (int)(addr / REGION_SIZE);
	allocAssert(id >= 0 && (Size)id < MAX_REGIONS);
	
	return id;
}

/*******************  FUNCTION  *********************/
Region* RegionRegistry::setupNewRegion ( Addr ptr )
{
	Region * region = NULL;
	
	//errors
	allocAssume(ptr < PHYS_MAX_ADDR,"Address is too big to be registered into the global region registry !");
	
	//get region ID
	int id = getRegionId(ptr);
	allocAssert(id >= 0);

	//ensure init and take the lock
	START_CRITICAL(spinlock)
		//check if already mapped, otherwise, to nothing
		if (regions[id] == NULL)
		{
			/** @todo this may be better to hardly control this address choice, maybe use the allocator when a first chain is available. **/
			region = (Region*)OS::mmap(NULL,sizeof(Region));
			/** @todo PARALLEL check for atomic operation instead of lock **/
			regions[id] = region;
		} else {
			region = regions[id];
		}
	END_CRITICAL

	//return pointer to the region
	return region;
}

/*******************  FUNCTION  *********************/
Region* RegionRegistry::getRegion ( Addr ptr )
{
	int id = getRegionId(ptr);
	return regions[id];
}

/*******************  FUNCTION  *********************/
RegionEntry* RegionRegistry::getRegionEntry ( Addr ptr , bool createIdNotExist)
{
	 //vars
	Addr id;
	
	if (ptr > PHYS_MAX_ADDR)
	{
		allocWarning("Invalid address range in request for region registry : %p !",ptr);
		return NULL;
	}

	//get the local region
	struct Region * region = getRegion(ptr);
	if (region == NULL && createIdNotExist)
		region = setupNewRegion(ptr);
	
	if (region == NULL)
		return NULL;

	//compute ID in regino
	id = (((Addr)ptr) % REGION_SIZE) / REGION_SPLITTING;

	//check
	allocAssert(id >= 0);
	allocAssert(id < REGION_ENTRIES);

	//return id
	return region->entries + id;

}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* RegionRegistry::getSegment ( void* ptr )
{
	//get addr for search
	Addr addr = (Addr)ptr;
	
	RegionEntry * entry = getRegionEntry(addr,false);
	
	//try previous
	if (entry == NULL || *entry == NULL || *entry > ptr)
		entry = getRegionEntry(addr-REGION_SPLITTING,false);
	
	if (entry == NULL)
		return NULL;
	else if (*entry == NULL || *entry > ptr)
		return NULL;

	//check
	if ((*entry)->contain(ptr))
		return *entry;
	else
		return NULL;
}

/*******************  FUNCTION  *********************/
void RegionRegistry::remove ( void * ptr )
{
	//get the entry to remove
	RegionSegmentHeader * segment = getSegment(ptr);
	//TODO assume
	allocAssert(segment != NULL);
	allocAssert(segment->contain(ptr));
	
	remove(segment);
}

/*******************  FUNCTION  *********************/
void RegionRegistry::remove ( RegionSegmentHeader* segment )
{
	Addr ptr = (Addr) segment;
	Size entrySize = segment->getTotalSize();

	//errors
	allocAssert( segment != NULL);

	//TODO can be optimized by playing with REGIN size multiples with ++
	for (Size offset = 0 ; offset < entrySize ; offset += REGION_SPLITTING)
	{
		RegionEntry * localEntry = getRegionEntry(ptr + offset,false);
		//TODO assume
		allocAssert(localEntry != NULL);
		if (*localEntry == segment )
			*localEntry = NULL;
	}
}

/*******************  FUNCTION  *********************/
void RegionRegistry::setEntry ( RegionSegmentHeader* segment )
{
	Addr ptr = (Addr)segment;

	//errors
	allocAssert(segment != NULL);
	allocAssert(segment->getManager() != NULL);
	allocAssert(segment->contain(segment));
	//allocAssert(! (chain->flags & SCTK_ALLOC_CHAIN_DISABLE_REGION_REGISTER));

	//warn if too small
	if (segment->getTotalSize() < REGION_SPLITTING)
		allocAssert(false);
	//TODO wanring
	//warning("Caution, using macro blocs smaller than SCTK_MACRO_BLOC_SIZE is dangerous, check usage of flag SCTK_ALLOC_CHAIN_DISABLE_REGION_REGISTER.");
	
	//TODO can be optimized by playing with REGIN size multiples with ++
	Size entrySize = segment->getTotalSize();
	Addr endPtr = ceilToPowOf2(ptr + entrySize,REGION_SPLITTING);
	for (Size offset = 0 ; ptr + offset < endPtr ; offset += REGION_SPLITTING)
	{
		RegionEntry * localEntry = getRegionEntry(ptr + offset,true);
		//TODO assume
		allocAssert(localEntry != NULL);
		allocAssert(*localEntry == NULL);
		*localEntry = segment;
	}
}

/*******************  FUNCTION  *********************/
bool RegionRegistry::hasEntry ( void* ptr )
{
	return getSegment(ptr) != NULL;
}

/*******************  FUNCTION  *********************/
bool RegionRegistry::isOnSplttingLimit ( Addr ptr ) const
{
	//TODO optimize for power of 2
	allocAssert( (ptr % REGION_SPLITTING == 0) == ((ptr & (REGION_SPLITTING-1)) == 0));
	return ((ptr & (REGION_SPLITTING-1)) == 0);
}

/*******************  FUNCTION  *********************/
void RegionRegistry::clearAll ( void )
{
	START_CRITICAL(spinlock)
		for (Size i = 0 ; i < MAX_REGIONS ; i++)
		{
			if (regions[i] != NULL)
			{
				//TODO warning
				//if (regions[i]->isEmpty() == false)
				//	allocAssert(false);
				
				//free mem
				OS::mmap(regions[i],sizeof(Region));
				
				//mark as removed
				regions[i] = NULL;
			}
		}
	END_CRITICAL
}

/*******************  FUNCTION  *********************/
RegionRegistry::RegionRegistry ( void )
{
	memset(regions,0,sizeof(regions));
}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* RegionRegistry::getSegmentSafe ( void* ptr )
{
	RegionSegmentHeader* res = getSegment(ptr);
	allocCondWarning(res != NULL && ALLOC_DO_WARNING,"Failed to find the requested adress in global registry, it may not be allocated by this allocator.");
	return res;
}

/*******************  FUNCTION  *********************/
RegionRegistry::~RegionRegistry ( void )
{
	clearAll();
}

/*******************  FUNCTION  *********************/
void Region::clear ( void )
{
	allocAssert(this != NULL);
	memset(entries,0,sizeof(entries));
}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* RegionRegistry::setEntry ( void* ptr, Size totalSize, IChunkManager* manager )
{
	//errors
	allocAssert(ptr != NULL);
	allocAssert(totalSize >= REGION_SPLITTING);
	allocAssert(manager != NULL);

	RegionSegmentHeader * res = RegionSegmentHeader::setup(ptr,totalSize,manager);
	setEntry(res);
	return res;
}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* RegionSegmentHeader::getSegment ( void* ptr )
{
	RegionSegmentHeader * segment = (RegionSegmentHeader *)ptr;
	return segment - 1;
}

/*******************  FUNCTION  *********************/
ListElement* RegionSegmentHeader::getListHandler ( void )
{
	allocAssert(this != NULL);
	return (ListElement*) (this+1);
}

/*******************  FUNCTION  *********************/
RegionSegmentHeader * RegionSegmentHeader::getFromListHandler ( ListElement* list )
{
	RegionSegmentHeader * res = NULL;
	if (list != NULL)
		res = ((RegionSegmentHeader*)list) - 1;
	return res;
}

/*******************  FUNCTION  *********************/
void RegionSegmentHeader::setManager ( IChunkManager* manager )
{
	this->manager = manager;
}

void RegionRegistry::unmapAllMemory ( void )
{
	START_CRITICAL(spinlock)
		//loop on all regions
		for (unsigned int regionId = 0 ; regionId < MAX_REGIONS ; regionId++)
		{
			if (regions[regionId] != NULL)
			{
				//cleanup the region
				regions[regionId]->unmapRegisteredMemory();
				//unmap it
				OS::munmap(regions[regionId],sizeof(Region));
			}
		}
	END_CRITICAL
}

/*******************  FUNCTION  *********************/
void Region::unmapRegisteredMemory ( void )
{
	RegionEntry last = NULL;

	//loop on all segments to free them
	for (unsigned int i = 0 ; i < REGION_ENTRIES ; i++ )
	{
		if (entries[i] != NULL && entries[i] != last)
			OS::munmap(entries[i],entries[i]->getTotalSize());
		last = entries[i];
	}
}

};
