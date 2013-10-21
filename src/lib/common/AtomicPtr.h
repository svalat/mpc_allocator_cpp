/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef LINUX_ATOMIC_PTR_H
#define LINUX_ATOMIC_PTR_H

/********************  HEADERS  *********************/
#include <stdlib.h>
#include <Atomics.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

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

};

/********************  HEADERS  *********************/
#include "AtomicPtr_impl.h"

#endif //LINUX_ATOMIC_PTR_H
