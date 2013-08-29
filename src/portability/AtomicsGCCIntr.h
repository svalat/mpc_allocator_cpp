#ifndef ATOMICS_GCC_INSTR_H
#define ATOMICS_GCC_INSTR_H

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

#endif //ATOMICS_GCC_INSTR_H
