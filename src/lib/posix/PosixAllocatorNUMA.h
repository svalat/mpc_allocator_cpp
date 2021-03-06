/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef POSIX_ALLOCATOR_NUMA_H
#define POSIX_ALLOCATOR_NUMA_H

/********************  HEADERS  *********************/
#include "PosixAllocator.h"
#include "Topo.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CLASS  **********************/
class PosixAllocatorNUMA : public PosixAllocator
{
	public:
		PosixAllocatorNUMA(bool useStrict = false);
		  void* numaAlloc(int numaId, size_t size);
		virtual void postInit(void);
		void migrate(void);
	protected:
		virtual IMMSource * getMMSource(void);
	private:
		PosixAllocatorLocal internalAllocator;
		IMMSource ** numaMMSources;
		IMMSource ** numaStrictMMSources;
		IAllocator ** numaStrictAlloc;
		Topo * topo;
		int numaTopoId;
		int numaTopoCnt;
		bool useStrict;
};

};

#endif //POSIX_ALLOCATOR_NUMA_H
