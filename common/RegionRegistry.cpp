/********************  HEADERS  *********************/
#include <cstring>
#include "RegionRegistry.h"
#include "OS.h"

/*******************  FUNCTION  *********************/
RegionSegmentHeader* RegionSegmentHeader::setup ( void* ptr, Size totalSize, IChunkManager* manager )
{
	//TODO maybe some assume
	assert(ptr != NULL);
	assert( totalSize >= REGION_SPLITTING);
	assert(manager != NULL);
	assert(totalSize >= sizeof(RegionSegmentHeader));

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
	assert(this != NULL);
	return manager;
}

/*******************  FUNCTION  *********************/
Size RegionSegmentHeader::getTotalSize ( void ) const
{
	assert(this != NULL);
	return totalSize;
}

/*******************  FUNCTION  *********************/
Size RegionSegmentHeader::getInnerSize ( void ) const
{
	assert(this != NULL);
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
	assert(addr != ADDR_NULL);
	assert(addr < PHYS_MAX_ADDR);
	
	/** @TODO can be optimize if we consider power of 2 **/
	int id = (int)(addr / REGION_SIZE);
	assert(id >= 0 && (Size)id < MAX_REGIONS);
	
	return id;
}

/*******************  FUNCTION  *********************/
Region* RegionRegistry::setupNewRegion ( Addr ptr )
{
	Region * region = NULL;
	
	//get region ID
	int id = getRegionId(ptr);
	assert(id >= 0);

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

	//get the local region
	struct Region * region = getRegion(ptr);


	//compute ID in regino
	id = (((Addr)ptr) % REGION_SIZE) / REGION_SPLITTING;

	//check
	assert(id >= 0);
	assert(id < REGION_ENTRIES);

	//return id
	return region->entries + id;

}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* RegionRegistry::getSegment ( void* ptr )
{
	//get addr for search
	Addr addr = (Addr)ptr;
	if (!isOnSplttingLimit(addr))
		addr += REGION_SPLITTING;
	
	RegionEntry * entry = getRegionEntry(addr,false);
	
	//no entry
	if (entry == NULL)
		return NULL;

	//check
	assert(*entry != NULL);
	assert((*entry)->contain(ptr));
	
	//return
	return *entry;
}

/*******************  FUNCTION  *********************/
void RegionRegistry::remove ( void * ptr )
{
	//get the entry to remove
	RegionSegmentHeader * segment = getSegment(ptr);
	//TODO assume
	assert(segment != NULL);
	assert(segment->contain(ptr));
	
	remove(segment);
}

/*******************  FUNCTION  *********************/
void RegionRegistry::remove ( RegionSegmentHeader* segment )
{
	Addr ptr = (Addr) segment;
	Size entrySize = segment->getTotalSize();

	//errors
	assert( segment != NULL);

	//TODO can be optimized by playing with REGIN size multiples with ++
	for (Size offset = 0 ; offset < entrySize ; offset += REGION_SPLITTING)
	{
		RegionEntry * localEntry = getRegionEntry(ptr + offset,false);
		//TODO assume
		assert(localEntry != NULL);
		if (*localEntry == segment )
			*localEntry = NULL;
	}
}

/*******************  FUNCTION  *********************/
void RegionRegistry::setEntry ( RegionSegmentHeader* segment )
{
	Addr ptr = (Addr)segment;

	//errors
	assert(segment != NULL);
	assert(segment->getManager() != NULL);
	assert(segment->contain(segment));
	//assert(! (chain->flags & SCTK_ALLOC_CHAIN_DISABLE_REGION_REGISTER));

	//warn if too small
	if (segment->getTotalSize() < REGION_SPLITTING)
		assert(false);
	//TODO wanring
	//warning("Caution, using macro blocs smaller than SCTK_MACRO_BLOC_SIZE is dangerous, check usage of flag SCTK_ALLOC_CHAIN_DISABLE_REGION_REGISTER.");
	
	//TODO can be optimized by playing with REGIN size multiples with ++
	Size entrySize = segment->getTotalSize();
	for (Size offset = 0 ; offset < entrySize ; offset += REGION_SPLITTING)
	{
		if (offset > 0 || isOnSplttingLimit(ptr+offset))
		{
			RegionEntry * localEntry = getRegionEntry(ptr + offset,true);
			//TODO assume
			assert(localEntry != NULL);
			assert(*localEntry == NULL);
			*localEntry = segment;
		}
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
	assert( (ptr % REGION_SPLITTING == 0) == ((ptr & (~(REGION_SPLITTING-1))) == 0));
	return ((ptr & (~(REGION_SPLITTING-1))) == 0);
}

/*******************  FUNCTION  *********************/
void RegionRegistry::clearAll ( void )
{
	START_CRITICAL(spinlock)
		for (Size i = 0 ; i < MAX_REGIONS ; i++)
		{
			if (regions[i] != NULL)
			{
				//TODO assume
				if (regions[i]->isEmpty() == false)
					assert(false);
				
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
	//TODO assume
	if (res == NULL)
		assert(false);
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
	assert(this != NULL);
	memset(entries,0,sizeof(entries));
}
