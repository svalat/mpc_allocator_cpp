#ifndef CHUNK_ALLOCATOR_H
#define CHUNK_ALLOCATOR_H

/********************  HEADERS  *********************/
#include "MediumFreePool.h"
#include "IAllocator.h"
#include "Spinlock.h"

/*********************  CLASS  **********************/
class MediumAllocator : public IChunkManager
{
	public:
		MediumAllocator(bool useLocks,IAllocator * memorySource = NULL);
		//required public interface
		void * malloc(size_t size,size_t align = BASIC_ALIGN,bool * zeroFilled = NULL);
		virtual void free ( void* ptr );
		virtual size_t getInnerSize ( void* ptr );
		virtual size_t getRequestedSize ( void* ptr );
		virtual size_t getTotalSize ( void* ptr );
		virtual void* realloc ( void* ptr, size_t size );
	private:
		MediumChunk * refill(size_t size,bool * zeroFilled = NULL);
		MediumChunk * split(MediumChunk * chunk);
	private:
		MediumFreePool pool;
		Spinlock spinlock;
		IAllocator * memorySource;
		bool useLocks;
};

#endif //CHUNK_ALLOCATOR_H
