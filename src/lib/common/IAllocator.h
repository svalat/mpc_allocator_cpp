#ifndef IALLOCATOR_H
#define IALLOCATOR_H

/********************  HEADERS  *********************/
#include <stdlib.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  TYPES  **********************/
class RegionSegmentHeader;

/*********************  CLASS  **********************/
class IChunkManager
{
	public:
		IChunkManager(IChunkManager * parent = NULL) {this->parent = parent;};
		virtual void free(void * ptr) = 0;
		virtual void * realloc(void * ptr,size_t size) = 0;
		virtual size_t getInnerSize(void * ptr) = 0;
		virtual size_t getTotalSize(void * ptr) = 0;
		virtual size_t getRequestedSize(void * ptr) = 0;
		virtual void hardChecking(void) = 0;
		//remote part for thread aware
		virtual bool isThreadSafe(void) const = 0;
		virtual void remoteFree(void * ptr) = 0;
		void setParentChunkManager(IChunkManager * parent) { this->parent = parent;};
		IChunkManager * getParentChunkManager(void) {return parent;};
	private:
		IChunkManager * parent;
};

/*********************  CLASS  **********************/
class IAllocator : public IChunkManager
{
	public:
		virtual void * malloc(size_t size,size_t align,bool * zeroFilled = NULL) = 0;
		virtual bool isLocalChunkManager(IChunkManager * manager) = 0;
		virtual void flushRemote(void) = 0;
};

/*********************  CLASS  **********************/
class IMMSource
{
	public:
		virtual RegionSegmentHeader * map(size_t innerSize, bool * zeroFilled, IChunkManager * manager) = 0;
		virtual RegionSegmentHeader * remap(RegionSegmentHeader * oldSegment,size_t newInnerSize, IChunkManager * manager) = 0;
		virtual void unmap(RegionSegmentHeader * segment) = 0;
		//virtual bool haveEfficientRemap(void) const = 0;
};

};

#endif //IALLOCATOR_H
