#ifndef LINUX_ATOMIC_PTR_H
#define LINUX_ATOMIC_PTR_H

/********************  HEADERS  *********************/
#include <Atomics.h>

/*********************  CLASS  **********************/
template <class T>
class AtomicPtr
{
	public:
		AtomicPtr(T * ptr = NULL);
		T * atomicLoad(void);
		void atomicStore(T * newValue);
		T * atomicSwap(T * newValue);
	private:
		AtomicPtr(AtomicPtr<T> & orig);
		AtomicPtr & operator=(AtomicPtr<T> & newValue);
	private:
		AtomicPtrLL value;
};

/********************  HEADERS  *********************/
#include "AtomicPtr_impl.h"

#endif //LINUX_ATOMIC_PTR_H
