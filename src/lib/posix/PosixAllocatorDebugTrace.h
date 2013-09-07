#ifndef POSIX_ALLOCATOR_DEBUG_TRACE_H
#define POSIX_ALLOCATOR_DEBUG_TRACE_H

/********************  HEADERS  *********************/
#include "PosixAllocator.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CLASS  **********************/
class PosixAllocatorDebugTrace : public PosixAllocator
{
	public:
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
};

};

#endif //POSIX_ALLOCATOR_DEBUG_TRACE_H
