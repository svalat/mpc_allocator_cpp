/********************  HEADERS  *********************/
#include "Spinlock.h"

/*******************  FUNCTION  *********************/
Spinlock::Spinlock ( void )
{
	pthread_spin_init(&spinlock,PTHREAD_PROCESS_PRIVATE);
}

/*******************  FUNCTION  *********************/
Spinlock::~Spinlock ( void )
{
	pthread_spin_destroy(&spinlock);
}

/*******************  FUNCTION  *********************/
void Spinlock::lock ( void )
{
	pthread_spin_lock(&spinlock);
}

/*******************  FUNCTION  *********************/
void Spinlock::unlock ( void )
{
	pthread_spin_unlock(&spinlock);
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
	unlock();
}

/*******************  FUNCTION  *********************/
void Spinlock::TakeLock::unlock ( void )
{
	if (lock != NULL)
		lock->unlock();
	lock = NULL;
}
