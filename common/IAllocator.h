#ifndef IALLOCATOR_H
#define IALLOCATOR_H

/********************  HEADERS  *********************/
#include <stdlib.h>

/*********************  TYPES  **********************/
class RegionSegmentHeader;

/*********************  CLASS  **********************/
class IChunkManager
{
	public:
		virtual void free(void * ptr) = 0;
		virtual void * realloc(void * ptr,size_t size) = 0;
		virtual size_t getInnerSize(void * ptr) = 0;
		virtual size_t getTotalSize(void * ptr) = 0;
		virtual size_t getRequestedSize(void * ptr) = 0;
};

/*********************  CLASS  **********************/
class IAllocator : public IChunkManager
{
	public:
		virtual void * malloc(size_t size,size_t align,bool * zeroFilled = NULL) = 0;
};

/*********************  CLASS  **********************/
class IMMSource
{
	public:
		virtual RegionSegmentHeader * map(size_t size, bool * zeroFilled) = 0;
		virtual RegionSegmentHeader * remap(RegionSegmentHeader * oldSegment,size_t newSize) = 0;
		virtual void unmap(RegionSegmentHeader * segment) = 0;
		virtual bool haveEfficientRemap(void) const = 0;
};

#endif //IALLOCATOR_H
