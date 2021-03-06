/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdlib>
#include "PosixAllocator.h"
#include "PosixAllocatorDebugTrace.h"
#include "PosixAllocatorFileTrace.h"
#include "PosixAllocatorStd.h"
#include "PosixAllocatorNUMA.h"

/**********************  USING  *********************/
using namespace MPCAllocator;

/*********************  TYPES  **********************/
typedef PosixAllocatorNUMA SelectedPosixAllocator;
// typedef PosixAllocator SelectedPosixAllocator;
// typedef PosixAllocatorDebugTrace SelectedPosixAllocator;
// typedef PosixAllocatorFileTrace SelectedPosixAllocator;
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
DLL_PUBLIC void *malloc(size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	void * ptr = gblAllocator->malloc(size);
	allocAssert((Addr)ptr % BASIC_ALIGN == 0);
	allocAssert(size <= gblAllocator->getInnerSize(ptr));
	
	return ptr;
}

/*******************  FUNCTION  *********************/
DLL_PUBLIC void free(void *ptr)
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
DLL_PUBLIC void *calloc(size_t nmemb, size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	void * ptr = gblAllocator->calloc(nmemb,size);
	if (size >= BASIC_ALIGN)
		allocAssert((Addr)ptr % BASIC_ALIGN == 0 || (Addr)ptr % size == 0);
	allocAssert(size * nmemb <= gblAllocator->getInnerSize(ptr));
	
	return ptr;
}

/*******************  FUNCTION  *********************/
DLL_PUBLIC void *realloc(void *ptr, size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	ptr = gblAllocator->realloc(ptr,size);
	allocAssert((Addr)ptr % BASIC_ALIGN == 0);
	allocAssert(size <= gblAllocator->getInnerSize(ptr));
	
	return ptr;
}

/*******************  FUNCTION  *********************/
DLL_PUBLIC int posix_memalign(void **memptr, size_t alignment, size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	int res = gblAllocator->posix_memalign(memptr,alignment,size);
	allocAssert((Addr)*memptr % alignment == 0);
	allocAssert(size <= gblAllocator->getInnerSize(*memptr));
	
	return res;
}

/*******************  FUNCTION  *********************/
DLL_PUBLIC void *aligned_alloc(size_t alignment, size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//do and check
	void * ptr = gblAllocator->aligned_alloc(alignment,size);
	allocAssert((Addr)ptr % alignment == 0);
	allocAssert(size <= gblAllocator->getInnerSize(ptr));
	
	//forward action
	return ptr;
}

/*******************  FUNCTION  *********************/
DLL_PUBLIC void *valloc(size_t size)
{ 
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	void * ptr = gblAllocator->valloc(size);
	allocAssert((Addr)ptr % PAGE_SIZE == 0);
	allocAssert(size <= gblAllocator->getInnerSize(ptr));
	
	return ptr;
}

/*******************  FUNCTION  *********************/
DLL_PUBLIC void *memalign(size_t alignment, size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	void  *ptr = gblAllocator->memalign(alignment,size);
	allocAssert((Addr)ptr % alignment == 0);
	allocAssert(size <= gblAllocator->getInnerSize(ptr));
	
	return ptr;
}

/*******************  FUNCTION  *********************/
DLL_PUBLIC void *pvalloc(size_t size)
{
	//check init
	if (gblAllocator == NULL)
		allocInit();
	
	//forward action
	void * ptr = gblAllocator->pvalloc(size);
	allocAssert((Addr)ptr % PAGE_SIZE == 0);
	allocAssert(size <= gblAllocator->getInnerSize(ptr));
	
	return ptr;
}
