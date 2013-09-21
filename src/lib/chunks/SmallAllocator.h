#ifndef SMALL_CHUNK_ALLOCATOR_H
#define SMALL_CHUNK_ALLOCATOR_H

/********************  HEADERS  *********************/
#include <cstdlib>
#include <Spinlock.h>
#include <SmallChunkRun.h>
#include <RegionRegistry.h>
#include <DoubleLinkList.h>
#include "SmallRunContainer.h"

/********************  MACRO  ***********************/
#define NB_SIZE_CLASS 9
#define SMALL_CHUNK_MAX_SIZE 128
//8 16 24 32 48 64 80 96 128

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  TYPES  **********************/
typedef DoubleLinkList<SmallRunContainer> SmallRunContainerList;

/*********************  CLASS  **********************/
class SmallAllocator : public IChunkManager
{
	public:
		SmallAllocator(bool useLocks = false,IMMSource * memorySource = NULL);
		//required public interface
		void * malloc(size_t size,size_t align = BASIC_ALIGN,bool * zeroFilled = NULL);
		virtual void free ( void* ptr );
		virtual size_t getInnerSize ( void* ptr );
		virtual size_t getRequestedSize ( void* ptr );
		virtual size_t getTotalSize ( void* ptr );
		virtual void* realloc ( void* ptr, size_t size );
		void fill(void * ptr, size_t size, RegionRegistry * registry);
	private:
		void fill(void * ptr, size_t size, RegionRegistry * registry,bool lock);
		int getSizeClass(size_t size) const;
		void refill(void);
		void markRunAsFree(SmallChunkRun * run);
		SmallChunkRun * findEmptyRun(void);
		SmallChunkRun * updateActivRunForSize( int sizeClass );
		SmallChunkRun * getActivRunForSize(int sizeClass);
		SmallChunkRun * getRun(void * ptr);
	private:
		Spinlock spinlock;
		IMMSource * memorySource;
		bool useLocks;
		SmallChunkRun * activRuns[NB_SIZE_CLASS];
		SmallChunkRunList inUse[NB_SIZE_CLASS];
		SmallRunContainerList containers;
};

};

#endif //SMALL_CHUNK_ALLOCATOR_H
