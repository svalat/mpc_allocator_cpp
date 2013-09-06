/********************  HEADERS  *********************/
#include <cstdlib>
#include "PosixAllocator.h"
#include "PosixAllocatorDebugTrace.h"
#include "PosixAllocatorFileTrace.h"
#include "PosixAllocatorStd.h"

/*********************  TYPES  **********************/
// typedef PosixAllocator SelectedPosixAllocator;
// typedef PosixAllocatorDebugTrace SelectedPosixAllocator;
typedef PosixAllocatorFileTrace SelectedPosixAllocator;
// typedef PosixAllocatorStd SelectedPosixAllocator;

/********************  GLOBALS  *********************/
static char gblAllocatorStructMem[sizeof(SelectedPosixAllocator)];
static SelectedPosixAllocator * gblAllocator = NULL;
static Locks::StaticInitLock gblAllocInitLock = LOCKS_INIT_MUTEX;

/*******************  FUNCTION  *********************/
static void allocInit(void)
{
	//check
	allocAssert(sizeof(*gblAllocator) == sizeof(gblAllocatorStructMem));
	
	//take lock
	Locks::lock(gblAllocInitLock);
	
	//check if another has done the job
	if (gblAllocator == NULL)
	{
		gblAllocator = new (gblAllocatorStructMem) SelectedPosixAllocator;
		gblAllocator->postInit();
	}
	
	//unlock
	Locks::unlock(gblAllocInitLock);
}

/*******************  FUNCTION  *********************/
void *malloc(size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	return gblAllocator->malloc(size);
}

/*******************  FUNCTION  *********************/
void free(void *ptr)
{
	//nothing to do
	if (ptr == NULL)
		return;
	
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	gblAllocator->free(ptr);
}

/*******************  FUNCTION  *********************/
void *calloc(size_t nmemb, size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	return gblAllocator->calloc(nmemb,size);
}

/*******************  FUNCTION  *********************/
void *realloc(void *ptr, size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	return gblAllocator->realloc(ptr,size);
}

/*******************  FUNCTION  *********************/
int posix_memalign(void **memptr, size_t alignment, size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	return gblAllocator->posix_memalign(memptr,alignment,size);
}

/*******************  FUNCTION  *********************/
void *aligned_alloc(size_t alignment, size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	return gblAllocator->aligned_alloc(alignment,size);
}

/*******************  FUNCTION  *********************/
void *valloc(size_t size)
{ 
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	return gblAllocator->valloc(size);
}

/*******************  FUNCTION  *********************/
void *memalign(size_t alignment, size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	return gblAllocator->memalign(alignment,size);
}

/*******************  FUNCTION  *********************/
void *pvalloc(size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	return gblAllocator->pvalloc(size);
}
