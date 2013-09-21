/********************  HEADERS  *********************/
#include "PosixAllocator.h"
#include "Debug.h"
#include <cstring>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
PosixAllocator::PosixAllocator ( void )
	:mmSource(&registry), mediumAlloc(true,&mmSource), smallAlloc(true,&mmSource)
{
	//mark as init
	isInit = true;
}

/*******************  FUNCTION  *********************/
void PosixAllocator::postInit ( void )
{

}

/*******************  FUNCTION  *********************/
void PosixAllocator::free ( void* ptr )
{
	//errors
	allocAssert(isInit);
	
	//trivial
	if (ptr == NULL)
		return;
	
	//get manager
	IChunkManager * chunkManager = getChunkManager(ptr);
	allocAssert(chunkManager != NULL);
	
	//free it
	if (chunkManager != NULL)
		chunkManager->free(ptr);
}

/*******************  FUNCTION  *********************/
void* PosixAllocator::malloc ( size_t size )
{
	//errors
	allocAssert(isInit);

	//to be compatible with glibc policy which didn't return NULL in this case.
	//otherwise we got crash in sed/grep/nano ...
	/** @todo Optimize by returning a specific fixed address instead of alloc size=1 **/
	if (size == 0)
		size = 1;
	
	//call internal malloc
	return internalMalloc(size);
}

/*******************  FUNCTION  *********************/
void* PosixAllocator::aligned_alloc ( size_t alignment, size_t size )
{
	//errors
	allocAssert(isInit);

	allocWarning("Not implemented");
	allocAssert(size % alignment == 0);
	return internalMalloc(size,alignment);
}

/*******************  FUNCTION  *********************/
void* PosixAllocator::calloc ( size_t nmemb, size_t size )
{
	//errors
	allocAssert(isInit);
	
	//do it
	void * res = internalMalloc(size*nmemb,BASIC_ALIGN,true);

	return res;
}

/*******************  FUNCTION  *********************/
IChunkManager* PosixAllocator::getChunkManager ( void* ptr )
{
	//errors
	allocAssert(isInit);

	//search region segment
	RegionSegmentHeader * segment = registry.getSegmentSafe(ptr);
	allocAssert(segment != NULL);
	if (segment == NULL)
		return NULL;
	
	//get manager
	allocAssert(segment->contain(ptr));
	IChunkManager * chunkManager = segment->getManager();
	
	return chunkManager;
}

/*******************  FUNCTION  *********************/
size_t PosixAllocator::getInnerSize ( void* ptr )
{
	//errors
	allocAssert(isInit);

	//trivial
	if (ptr == NULL)
		return 0;

	//get manager
	IChunkManager * chunkManager = getChunkManager(ptr);
	allocAssert(chunkManager != NULL);

	//get size
	if (chunkManager == NULL)
		return 0;
	else
		return chunkManager->getInnerSize(ptr);
}

/*******************  FUNCTION  *********************/
size_t PosixAllocator::getRequestedSize ( void* ptr )
{
	//errors
	allocAssert(isInit);

	//trivial
	if (ptr == NULL)
		return 0;

	//get manager
	IChunkManager * chunkManager = getChunkManager(ptr);
	allocAssert(chunkManager != NULL);
	
	//get size
	if (chunkManager == NULL)
		return 0;
	else
		return chunkManager->getRequestedSize(ptr);
}

/*******************  FUNCTION  *********************/
size_t PosixAllocator::getTotalSize ( void* ptr )
{
	//errors
	allocAssert(isInit);

	//trivial
	if (ptr == NULL)
		return 0;

	//get manager
	IChunkManager * chunkManager = getChunkManager(ptr);
	allocAssert(chunkManager != NULL);
	
	//get size
	if (chunkManager == NULL)
		return 0;
	else
		return chunkManager->getTotalSize(ptr);
}

/*******************  FUNCTION  *********************/
void* PosixAllocator::memalign ( size_t alignment, size_t size )
{
	//errors
	allocAssert(isInit);

	//do it
	allocWarning("Not implemented");
	void * res = internalMalloc(size,alignment);
	
	return res;
}

/*******************  FUNCTION  *********************/
int PosixAllocator::posix_memalign ( void** memptr, size_t alignment, size_t size )
{
	//errors
	allocAssert(isInit);

	//to avoid issues like for malloc
	if (size == 0)
		size = 1;
	
	//error
	//TODO fill perror
	if (memptr == NULL)
		return -1;
	
	allocWarning("Not implemented");
	*memptr = internalMalloc(size,alignment);
	
	//TODO fill perror
	if (*memptr == NULL)
		return -1;
	else
		return 0;
}

/*******************  FUNCTION  *********************/
void* PosixAllocator::internalMalloc ( size_t size, size_t alignement, bool requireZero )
{
	//errors
	allocAssert(isInit);

	//var
	bool zeroStatus = requireZero;
	void * res = NULL;
	
	//round size
	if (size <= SMALL_CHUNK_MAX_SIZE)
		res = smallAlloc.malloc(size,alignement,&zeroStatus);
	else
		res = mediumAlloc.malloc(size,alignement,&zeroStatus);
	
	//if need reset
	//TODO use optim here
	if (res != NULL && requireZero == true && zeroStatus == false)
		memset(res,0,size);

	//TODO warning if NULl

	return res;
}

/*******************  FUNCTION  *********************/
void* PosixAllocator::pvalloc ( size_t size )
{
	//errors
	allocAssert(isInit);

	allocFatal("Not supported");
	return NULL;
}

/*******************  FUNCTION  *********************/
bool PosixAllocator::isDistantManager ( IChunkManager* manager )
{
	return (manager != &mediumAlloc && manager != &smallAlloc);
}

/*******************  FUNCTION  *********************/
void* PosixAllocator::realloc ( void* ptr, size_t size )
{
	//vars
	void * res = NULL;

	//errors
	allocAssert(isInit);
	
	//simple alloc
	if (ptr == NULL)
	{
		res = internalMalloc(size);
	} else if (size == 0) {
		this->free(ptr);
	} else {
		//get manager
		IChunkManager * chunkManager = getChunkManager(ptr);
		
		//manage bad realloc as we can
		if (chunkManager == NULL)
		{
			allocWarning("The old segment isn't managed by current memory allocator, try to copy, but create a memory leak and may segfault during unsafe copy.");
			res = malloc(size);
			memcpy(res,ptr,size);
		} else {
			//local and same class realloc, otherwise alloc/copy/free
			if ((size <= SMALL_CHUNK_MAX_SIZE && chunkManager == &smallAlloc) || (size > SMALL_CHUNK_MAX_SIZE && chunkManager == &mediumAlloc))
			{
				if (chunkManager == &smallAlloc)
				{
					res = smallAlloc.realloc(ptr,size);
				} else {
					res = mediumAlloc.realloc(ptr,size);
				}
			} else {
				void * new_ptr = internalMalloc(size);
				allocAssert(new_ptr != NULL);
				memcpy(new_ptr,ptr,min(size,chunkManager->getInnerSize(ptr)));
				chunkManager->free(ptr);
				res = new_ptr;
			}
		}
	}

	return res;
}

/*******************  FUNCTION  *********************/
void* PosixAllocator::valloc ( size_t size )
{
	//errors
	allocAssert(isInit);

	allocFatal("Not supported");
	return NULL;
}

};
