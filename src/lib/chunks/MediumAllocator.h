/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef CHUNK_ALLOCATOR_H
#define CHUNK_ALLOCATOR_H

/********************  HEADERS  *********************/
#include <IAllocator.h>
#include <Spinlock.h>
#include <RegionRegistry.h>
#include "MediumFreePool.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CLASS  **********************/
class MediumAllocator : public IChunkManager
{
	public:
		MediumAllocator(bool useLocks = false,IMMSource * memorySource = NULL);
		//required public interface
		void * malloc(size_t size,size_t align = BASIC_ALIGN,bool * zeroFilled = NULL);
		virtual void free ( void* ptr );
		virtual size_t getInnerSize ( void* ptr );
		virtual size_t getRequestedSize ( void* ptr );
		virtual size_t getTotalSize ( void* ptr );
		virtual void* realloc ( void* ptr, size_t size );
		virtual bool isThreadSafe ( void ) const;
		virtual void remoteFree ( void* ptr );
		virtual void hardChecking(void);
		void fill(void * ptr, size_t size,RegionRegistry * registry);
		void rebindMMSource(IMMSource * mmSource);
	private:
		MediumChunk * refill(size_t size,bool * zeroFilled = NULL);
		MediumChunk * split( MediumChunk* chunk, size_t innerSize );
	private:
		MediumFreePool pool;
		Spinlock spinlock;
		IMMSource * memorySource;
		bool useLocks;
		RegionRegistry * registry;
};

};

#endif //CHUNK_ALLOCATOR_H
