/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include "PosixAllocator.h"
#include "Debug.h"
#include <cstring>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/********************  GLOBALS  *********************/
__thread IAllocator * tlsLocalCurrentAllocator = NULL;
__thread IAllocator * tlsLocalDefaultAllocator = NULL;

/*******************  FUNCTION  *********************/
PosixAllocator::PosixAllocator ( void )
	:mmSource(&registry),internalAlloc(true,&mmSource)
{
	//mark as init
	isInit = true;
}

/*******************  FUNCTION  *********************/
void PosixAllocator::postInit ( void )
{

}

/*******************  FUNCTION  *********************/
IAllocator* PosixAllocator::initLocal ( void )
{
	//errors
	allocAssert(isInit);
	
	//alloc memory to store the local allocator
	void * mm = internalAlloc.malloc(sizeof(PosixAllocatorLocal));
	allocAssume(mm != NULL,"Fail to get memory from internal sub allocator to build the thread local allocator, maybe OOM.");
	
	//setup the posix local allocator
	PosixAllocatorLocal * localAlloc = new (mm)PosixAllocatorLocal(&registry,getMMSource());
	
	//make it as current default one
	tlsLocalCurrentAllocator = localAlloc;
	tlsLocalDefaultAllocator = localAlloc;
	
	return localAlloc;
}

/*******************  FUNCTION  *********************/
void PosixAllocator::resetDefaultAllocator(void)
{
	tlsLocalCurrentAllocator = tlsLocalDefaultAllocator;
}

/*******************  FUNCTION  *********************/
void PosixAllocator::setCurrentAllocator(IAllocator* alloctor)
{
	tlsLocalCurrentAllocator = alloctor;
}

/*******************  FUNCTION  *********************/
void PosixAllocator::flushRemote ( IAllocator* localAllocator )
{
	IAllocator * defaultLocalAllocator = tlsLocalDefaultAllocator;

	//local flush
	localAllocator->flushRemote();
	if (defaultLocalAllocator != localAllocator && defaultLocalAllocator != NULL)//maybe this is too much
		defaultLocalAllocator->flushRemote();
}

/*******************  FUNCTION  *********************/
void PosixAllocator::free ( void* ptr )
{
	//errors
	allocAssert(isInit);

	//fetch TLS locally for fast use
	IAllocator * localAllocator = tlsLocalCurrentAllocator;
	if (localAllocator != NULL)
		flushRemote(localAllocator);
	
	//trivial
	if (ptr == NULL)
		return;
	
	//get manager
	IChunkManager * chunkManager = getChunkManager(ptr);
	allocAssert(chunkManager != NULL);
	
	//free it
	if (isDistantManager(localAllocator,chunkManager))
		chunkManager->remoteFree(ptr);
	else if (chunkManager != NULL)
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
	
	//fetch TLS locally for fast use
	IAllocator * localAllocator = tlsLocalCurrentAllocator;
	
	//check for flush
	if (localAllocator != NULL)
		flushRemote(localAllocator);
	
	//check for init of local alloc
	if (localAllocator == NULL)
		localAllocator = initLocal();
	
	//zero setup
	bool zeroStatus = requireZero;
	
	//call
	void * res = localAllocator->malloc(size,alignement,&zeroStatus); 
	
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
bool PosixAllocator::isDistantManager (IAllocator * localAlloc, IChunkManager* manager )
{
	if (manager->isThreadSafe())
		return false;
	else if (localAlloc == NULL)
		return true;
	else
		return localAlloc->isLocalChunkManager(manager);
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
		//get manager and fetch TLS locally for fast use
		IChunkManager * chunkManager = getChunkManager(ptr);
		IAllocator * localAllocator = tlsLocalCurrentAllocator;
		
		//manage bad realloc as we can
		if (chunkManager == NULL)
		{
			//TODO : made this cas optional for resistant mode
			allocWarning("The old segment isn't managed by current memory allocator, try to copy, but create a memory leak and may segfault during unsafe copy.");
			res = internalMalloc(size);
			memcpy(res,ptr,size);
		} else if (isDistantManager(localAllocator,chunkManager)) {
			void * new_ptr = internalMalloc(size);
			allocAssert(new_ptr != NULL);
			memcpy(new_ptr,ptr,min(size,chunkManager->getInnerSize(ptr)));
			chunkManager->free(ptr);
			res = new_ptr;
		} else {
			IChunkManager * parentChunkManager = chunkManager->getParentChunkManager();
			if (parentChunkManager == NULL)
			{
				res = chunkManager->realloc(ptr,size);
			} else {
				allocAssert(parentChunkManager->getParentChunkManager() == NULL);
				res = parentChunkManager->realloc(ptr,size);
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

/*******************  FUNCTION  *********************/
void PosixAllocator::resetTLSForTest ( void )
{
	tlsLocalCurrentAllocator = NULL;
	tlsLocalDefaultAllocator = NULL;
}

/*******************  FUNCTION  *********************/
void PosixAllocator::cleanupRegisterdMemory ( void )
{
	registry.unmapAllMemory();
}

/*******************  FUNCTION  *********************/
void PosixAllocator::hardChecking ( void )
{
	if (tlsLocalDefaultAllocator != NULL)
		tlsLocalDefaultAllocator->hardChecking();
}

/*******************  FUNCTION  *********************/
IMMSource* PosixAllocator::getMMSource(void)
{
	return &mmSource;
}

};
