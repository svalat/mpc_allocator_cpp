/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef ALLOC_SPINLOCK_INLINE_H
#define ALLOC_SPINLOCK_INLINE_H

/********************  HEADERS  *********************/
#include "Spinlock.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
inline Spinlock::Spinlock ( void )
{
	Locks::init(spinlock);
}

/*******************  FUNCTION  *********************/
inline Spinlock::~Spinlock ( void )
{
	Locks::destroy(spinlock);
}

/*******************  FUNCTION  *********************/
inline void Spinlock::lock ( void )
{
	Locks::lock(spinlock);
}

/*******************  FUNCTION  *********************/
inline void Spinlock::unlock ( void )
{
	Locks::unlock(spinlock);
}

/*******************  FUNCTION  *********************/
inline Spinlock::TakeLock::TakeLock ( Spinlock& lock, bool test )
{
	if (test)
	{
		this->lock = &lock;
		lock.lock();
	} else {
		this->lock = NULL;
	}
}

/*******************  FUNCTION  *********************/
inline Spinlock::TakeLock::~TakeLock ( void )
{
	this->unlock();
}

/*******************  FUNCTION  *********************/
inline void Spinlock::TakeLock::unlock ( void )
{
	if (lock != NULL)
		lock->unlock();
	lock = NULL;
}

};

#endif //ALLOC_SPINLOCK_INLINE_H
