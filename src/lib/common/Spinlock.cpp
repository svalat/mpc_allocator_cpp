/********************  HEADERS  *********************/
#include "Spinlock.h"

/*******************  FUNCTION  *********************/
Spinlock::Spinlock ( void )
{
	Locks::init(spinlock);
}

/*******************  FUNCTION  *********************/
Spinlock::~Spinlock ( void )
{
	Locks::destroy(spinlock);
}

/*******************  FUNCTION  *********************/
void Spinlock::lock ( void )
{
	Locks::lock(spinlock);
}

/*******************  FUNCTION  *********************/
void Spinlock::unlock ( void )
{
	Locks::unlock(spinlock);
}

/*******************  FUNCTION  *********************/
Spinlock::TakeLock::TakeLock ( Spinlock& lock, bool test )
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
Spinlock::TakeLock::~TakeLock ( void )
{
	this->unlock();
}

/*******************  FUNCTION  *********************/
void Spinlock::TakeLock::unlock ( void )
{
	if (lock != NULL)
		lock->unlock();
	lock = NULL;
}
