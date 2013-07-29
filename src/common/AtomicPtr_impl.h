#ifndef ATOMIC_PTR_IMPL_H
#define ATOMIC_PTR_IMPL_H

/********************  HEADERS  *********************/
#include "AtomicPtr.h"

/*******************  FUNCTION  *********************/
template <class T>
inline AtomicPtr<T>::AtomicPtr(T * ptr)
{
	Atomics::atomicStore(&value,ptr);
}

/*******************  FUNCTION  *********************/
template <class T>
inline T * AtomicPtr<T>::atomicLoad()
{
	return (T*)Atomics::atomicLoad(&value);
}

/*******************  FUNCTION  *********************/
template <class T>
inline void AtomicPtr<T>::atomicStore( T * newValue )
{
	Atomics::atomicStore(&value,newValue);
}

/*******************  FUNCTION  *********************/
template <class T>
inline T * AtomicPtr<T>::atomicSwap ( T* newValue )
{
	return (T*)Atomics::atomicSwap(&value,newValue);
}

#endif
