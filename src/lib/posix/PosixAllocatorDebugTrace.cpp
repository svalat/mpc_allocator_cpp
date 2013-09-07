/********************  HEADERS  *********************/
#include "Debug.h"
#include "PosixAllocatorDebugTrace.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
void* PosixAllocatorDebugTrace::malloc ( size_t size )
{
	void * res = PosixAllocator::malloc(size);
	allocTrace("%p = malloc(%llu);",res,size);
	return res;
}

/*******************  FUNCTION  *********************/
void* PosixAllocatorDebugTrace::aligned_alloc ( size_t alignment, size_t size )
{
	void * res = PosixAllocator::aligned_alloc(alignment,size);
	allocTrace("%p = aligned_alloc(%llu,%llu);",res,alignment,size);
	return res;
}

/*******************  FUNCTION  *********************/
void* PosixAllocatorDebugTrace::calloc ( size_t nmemb, size_t size )
{
	void * res = PosixAllocator::calloc(nmemb,size);
	allocTrace("%p = calloc(%llu,%llu);",res,nmemb,size);
	return res;
}

/*******************  FUNCTION  *********************/
void PosixAllocatorDebugTrace::free ( void* ptr )
{
	if (ptr != NULL)
		allocTrace("free(%p);",ptr);
	PosixAllocator::free(ptr);
}

/*******************  FUNCTION  *********************/
void* PosixAllocatorDebugTrace::memalign ( size_t alignment, size_t size )
{
	void * res = PosixAllocator::memalign(alignment,size);
	allocTrace("%p = memalign(%llu,%llu);",res,alignment,size);
	return res;
}

/*******************  FUNCTION  *********************/
void* PosixAllocatorDebugTrace::realloc ( void* ptr, size_t size )
{
	void * res = PosixAllocator::realloc(ptr,size);
	allocTrace("%p = realloc(%p,%llu);",res,ptr,size);
	return res;
}

/*******************  FUNCTION  *********************/
int PosixAllocatorDebugTrace::posix_memalign ( void** memptr, size_t alignment, size_t size )
{
	int res = PosixAllocator::posix_memalign(memptr,alignment,size);
	allocTrace("%d = posix_memalign(%p,%llu,%llu); //%p",res,memptr,alignment,size,*memptr);
	return res;
}

/*******************  FUNCTION  *********************/
void* PosixAllocatorDebugTrace::pvalloc ( size_t size )
{
	void * res = PosixAllocator::pvalloc(size);
	allocTrace("%p = pvalloc(%llu);",res,size);
	return res;
}

/*******************  FUNCTION  *********************/
void* PosixAllocatorDebugTrace::valloc ( size_t size )
{
	void * res = PosixAllocator::valloc(size);
	allocTrace("%p = valloc(%llu);",res,size);
	return res;
}

};
