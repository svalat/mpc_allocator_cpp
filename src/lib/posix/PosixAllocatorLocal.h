/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef POSIX_ALLOCATOR_LOCAL_H
#define POSIX_ALLOCATOR_LOCAL_H

/********************  HEADERS  *********************/
#include "RegionRegistry.h"
#include <MPSCFQueue.h>
#include "DummyMMSource.h"
#include <CachedMMSource.h>
#include "MediumAllocator.h"
#include <SmallAllocator.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{
	
enum AllocatorClass
{
	ALLOCATOR_CLASS_SMALL,
	ALLOCATOR_CLASS_MEDIUM,
	ALLOCATOR_CLASS_HUGE
};

/*********************  CLASS  **********************/
class PosixAllocatorLocal : public IAllocator
{
	public:
		PosixAllocatorLocal(RegionRegistry * registry);
		virtual void postInit(void);
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
		//to be stored in list
		ListElement * getListHandler(void);
		static PosixAllocatorLocal * getFromListHandler(ListElement * list);
		//implement IAllocator
		virtual void flushRemote ( void );
		virtual bool isLocalChunkManager ( IChunkManager* manager );
		virtual bool isThreadSafe ( void ) const;
		virtual void* malloc ( size_t size, size_t align, bool* zeroFilled = 0 );
		virtual void remoteFree ( void* ptr );
		virtual void hardChecking(void);
	protected:
		void * internalMalloc (size_t size, size_t alignement = BASIC_ALIGN, bool requireZero = false);
		IChunkManager * getChunkManager(void * ptr);
		bool isDistantManager(IChunkManager * manager);
		AllocatorClass getSizeClass(Size innerSize);
	private:
		RegionRegistry * registry;
		CachedMMSource mmSource;
		MediumAllocator mediumAlloc;
		SmallAllocator smallAlloc;
		bool isInit;
		ListElement listHandler;
		MPSCFQueue rfq;
};

};

#endif //POSIX_ALLOCATOR_LOCAL_H
