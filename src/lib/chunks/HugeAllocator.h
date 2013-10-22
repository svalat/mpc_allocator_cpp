/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef HUGE_ALLOCATOR_H
#define HUGE_ALLOCATOR_H

/********************  HEADERS  *********************/
#include <IAllocator.h>
#include <RegionRegistry.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CLASS  **********************/
class HugeAllocator : public IChunkManager
{
	public:
		HugeAllocator(IMMSource * memorySource);
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
	private:
		IMMSource * memorySource;
};

};

#endif //HUGE_ALLOCATOR_H
