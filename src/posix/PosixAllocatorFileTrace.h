#ifndef POSIX_ALLOCATOR_FILE_TRACE_H
#define POSIX_ALLOCATOR_FILE_TRACE_H

/********************  HEADERS  *********************/
#include "PosixAllocator.h"
#include "AllocTraceStruct.h"

/*********************  CLASS  **********************/
class PosixAllocatorFileTrace : public PosixAllocator
{
	public:
		PosixAllocatorFileTrace(void);
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
	private:
		void writeEvent(TraceEntry & entry, TraceEntryType type);
		void writeAnswer(void * res = NULL);
	private:
		int fd;
		Spinlock lock;
		int nextThreadId;
};

#endif //POSIX_ALLOCATOR_FILE_TRACE_H
