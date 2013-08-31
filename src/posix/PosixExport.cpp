/********************  HEADERS  *********************/
#include <cstdlib>
#include "PosixAllocator.h"
#include "PosixAllocatorDebugTrace.h"

/********************  GLOBALS  *********************/
// static PosixAllocator gblAllocator;
static PosixAllocatorDebugTrace gblAllocator;

/*******************  FUNCTION  *********************/
void *malloc(size_t size)
{
	return gblAllocator.malloc(size);
}

/*******************  FUNCTION  *********************/
void free(void *ptr)
{
	gblAllocator.free(ptr);
}

/*******************  FUNCTION  *********************/
void *calloc(size_t nmemb, size_t size)
{
	return gblAllocator.calloc(nmemb,size);
}

/*******************  FUNCTION  *********************/
void *realloc(void *ptr, size_t size)
{
	return gblAllocator.realloc(ptr,size);
}

/*******************  FUNCTION  *********************/
int posix_memalign(void **memptr, size_t alignment, size_t size)
{
	return gblAllocator.posix_memalign(memptr,alignment,size);
}

/*******************  FUNCTION  *********************/
void *aligned_alloc(size_t alignment, size_t size)
{
	return gblAllocator.aligned_alloc(alignment,size);
}

/*******************  FUNCTION  *********************/
void *valloc(size_t size)
{ 
	return gblAllocator.valloc(size);
}

/*******************  FUNCTION  *********************/
void *memalign(size_t alignment, size_t size)
{
	return gblAllocator.memalign(alignment,size);
}

/*******************  FUNCTION  *********************/
void *pvalloc(size_t size)
{
	return gblAllocator.pvalloc(size);
}
