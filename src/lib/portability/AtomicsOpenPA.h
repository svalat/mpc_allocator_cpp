/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef ATOMICS_OPENPA_H
#define ATOMICS_OPENPA_H

/********************  HEADERS  *********************/
#include <opa_primitives.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  TYPES  **********************/
typedef OPA_ptr_t AtomicPtrLL;

/*********************  CLASS  **********************/
class AtomicsOpenPA
{
	public:
		static void * atomicLoad (AtomicPtrLL * ptr)                 {return OPA_load_ptr (ptr);         };
		static void   atomicStore(AtomicPtrLL * ptr,void * newValue) {       OPA_store_ptr(ptr,newValue);};
		static void * atomicSwap (AtomicPtrLL * ptr,void * newValue) {return OPA_swap_ptr (ptr,newValue);};
		static void   pause      (void)                              {       OPA_busy_wait();            };
};

};

#endif //ATOMICS_OPENPA_H
