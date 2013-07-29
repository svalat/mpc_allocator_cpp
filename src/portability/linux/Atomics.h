#ifndef LINUX_ATOMICS_H
#define LINUX_ATOMICS_H

/********************  HEADERS  *********************/
#include <opa_primitives.h>

/*********************  TYPES  **********************/
typedef OPA_ptr_t AtomicPtrLL;

/*********************  CLASS  **********************/
class OpenPAAtomics
{
	public:
		static void * atomicLoad (AtomicPtrLL * ptr)                 {return OPA_load_ptr (ptr);         };
		static void   atomicStore(AtomicPtrLL * ptr,void * newValue) {       OPA_store_ptr(ptr,newValue);};
		static void * atomicSwap (AtomicPtrLL * ptr,void * newValue) {return OPA_swap_ptr (ptr,newValue);};
		static void * pause      (void)                              {       OPA_busy_wait();            };
};

/*********************  TYPES  **********************/
typedef OpenPAAtomics Atomics;

#endif //LINUX_ATOMICS_H
