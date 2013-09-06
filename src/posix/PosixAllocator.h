#ifndef POSIX_ALLOCATOR_H
#define POSIX_ALLOCATOR_H

/********************  HEADERS  *********************/
#include "RegionRegistry.h"
#include "DummyMMSource.h"
#include "MediumAllocator.h"

/*********************  CLASS  **********************/
class PosixAllocator
{
	public:
		PosixAllocator(void);
		//The posix interface
		void   free ( void* ptr );
		void * malloc ( size_t size );
		void * realloc ( void* ptr, size_t size );
		void * calloc(size_t nmemb, size_t size);
		int    posix_memalign(void **memptr, size_t alignment, size_t size);
		void * aligned_alloc(size_t alignment, size_t size);
		void * valloc(size_t size);
		void * memalign(size_t alignment, size_t size);
		void * pvalloc(size_t size);
		//extra functions
		size_t getInnerSize ( void* ptr );
		size_t getRequestedSize ( void* ptr );
		size_t getTotalSize ( void* ptr );
		//compat with glibc
	protected:
		void * internalMalloc (size_t size, size_t alignement = BASIC_ALIGN, bool requireZero = false);
		IChunkManager * getChunkManager(void * ptr);
		bool isDistantManager(IChunkManager * manager);
	private:
		RegionRegistry registry;
		DummyMMSource mmSource;
		MediumAllocator mediumAlloc;
		bool isInit;
};

#endif //POSIX_ALLOCATOR_H
