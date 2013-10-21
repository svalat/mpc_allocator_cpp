/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef ATOMICS_GCC_INSTR_H
#define ATOMICS_GCC_INSTR_H

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  TYPES  **********************/
typedef void * volatile AtomicPtrLL;

/*********************  CLASS  **********************/
class AtomicsGCC
{
	public:
		static void * atomicLoad (AtomicPtrLL * ptr)                 {return *ptr ;                                      };
		static void   atomicStore(AtomicPtrLL * ptr,void * newValue) {       *ptr = newValue;                            };
		static void * atomicSwap (AtomicPtrLL * ptr,void * newValue) {return __sync_lock_test_and_set(ptr,newValue,ptr); };
		static void   pause      (void)                              {       do{}while(0);                               };
};

};

#endif //ATOMICS_GCC_INSTR_H
