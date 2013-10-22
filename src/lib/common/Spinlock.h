/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef SPINLOCKS_H
#define SPINLOCKS_H

/********************  HEADERS  *********************/
#include <Common.h>
#include <Locks.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CLASS  **********************/
class Spinlock
{
	public:
		class TakeLock
		{
			public:
				inline TakeLock(Spinlock & lock,bool test = true);
				inline ~TakeLock(void);
				inline void unlock(void);
			private:
				Spinlock * lock;
		};
	public:
		inline Spinlock(void);
		inline ~Spinlock(void);
		inline void lock(void);
		inline void unlock(void);
	private:
		Locks::Spinlock spinlock;
};

/********************  MACRO  ***********************/
#define OPTIONAL_CRITICAL(lock,test) do { Spinlock::TakeLock _local_##lock##__(lock,test);
#define START_CRITICAL(lock) { Spinlock::TakeLock _local_##lock##__(lock);
#define END_CRITICAL }while(0);

};

/********************  HEADERS  *********************/
#include "Spinlock_inline.h"

#endif //LOCKS
