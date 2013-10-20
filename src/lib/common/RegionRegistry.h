#ifndef REGION_REGISTRY_H
#define REGION_REGISTRY_H

/********************  HEADERS  *********************/
#include "Common.h"
#include "IAllocator.h"
#include "Spinlock.h"
#include "IDoubleLinkListElement.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/********************  MACRO  ***********************/
#define REGION_SPLITTING   MACRO_BLOC_SIZE
#define REGION_SIZE        (1024ULL*1024ULL*1024ULL*1024ULL)
#define REGION_ENTRIES     ( ( REGION_SIZE ) / REGION_SPLITTING )
#define PHYS_ADDR_BITS     48
#define PHYS_MAX_ADDR      ( 1UL << PHYS_ADDR_BITS )
#define MAX_REGIONS        ( PHYS_MAX_ADDR / REGION_SIZE )

/*********************  CLASS  **********************/
class RegionSegmentHeader : public IDoubleLinkListeElement
{
	public:
		static RegionSegmentHeader * setup(void * ptr,Size totalSize,IChunkManager * manager);
		IChunkManager * getManager(void);
		Size getTotalSize(void) const;
		Size getInnerSize(void) const;
		bool contain(void * ptr) const;
		void * getPtr(void);
		void setManager(IChunkManager * manager);
		static RegionSegmentHeader * getSegment(void * ptr);
		ListElement * getListHandler(void);
		static RegionSegmentHeader * getFromListHandler(ListElement * list);
	private:
		RegionSegmentHeader(void);
		RegionSegmentHeader(RegionSegmentHeader & orig);
		~RegionSegmentHeader(void);
	private:
		Size totalSize;
		IChunkManager * manager;
};

/*********************  TYPES  **********************/
typedef RegionSegmentHeader * RegionEntry;

/*********************  STRUCT  *********************/
struct Region
{
	void clear(void);
	bool isEmpty(void) const;
	void unmapRegisteredMemory(void);
	RegionEntry entries[REGION_ENTRIES];
};

/*********************  CLASS  **********************/
class RegionRegistry
{
	public:
		RegionRegistry(void);
		~RegionRegistry(void);
		RegionSegmentHeader * setEntry( void * ptr,Size totalSize,IChunkManager * manager);
		void setEntry( RegionSegmentHeader * segment );
		void remove(void * ptr);
		void remove( RegionSegmentHeader* segment );
		bool hasEntry(void * ptr);
		RegionSegmentHeader * getSegment(void * ptr);
		RegionSegmentHeader * getSegmentSafe(void * ptr);
		void unmapAllMemory(void);
	private:
		Region * setupNewRegion( Addr ptr );
		Region * getRegion( Addr ptr );
		RegionEntry * getRegionEntry( Addr ptr, bool createIdNotExist );
		void clearAll(void);
		int getRegionId(Addr addr);
		bool isOnSplttingLimit(Addr ptr) const;
	private:
		RegionRegistry(RegionRegistry & orig);
	private:
		Region * regions[MAX_REGIONS];
		Spinlock spinlock;
};

};

#endif //REGION_REGISTRY_H
