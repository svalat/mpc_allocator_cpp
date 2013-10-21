/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef POSIX_ALLOCATOR_H
#define POSIX_ALLOCATOR_H

/********************  HEADERS  *********************/
#include <pthread.h>
#include "RegionRegistry.h"
#include "DummyMMSource.h"
#include "CachedMMSource.h"
#include "PosixAllocatorLocal.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CLASS  **********************/
class PosixAllocator
{
	public:
		PosixAllocator(void);
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
		void resetTLSForTest(void);
		void cleanupRegisterdMemory(void);
		//to help debug
		void hardChecking(void);
	protected:
		IAllocator * initLocal(void);
		void * internalMalloc (size_t size, size_t alignement = BASIC_ALIGN, bool requireZero = false);
		IChunkManager * getChunkManager(void * ptr);
		bool isDistantManager(IAllocator * localAlloc,IChunkManager * manager);
		void flushRemote(IAllocator * localAllocator);
	private:
		RegionRegistry registry;
		CachedMMSource mmSource;
		MediumAllocator internalAlloc;
		DoubleLinkList<PosixAllocatorLocal> inuse;
		DoubleLinkList<PosixAllocatorLocal> unused;
		bool isInit;
};

};

#endif //POSIX_ALLOCATOR_H
