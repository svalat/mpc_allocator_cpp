#ifndef SPINLOCKS_H
#define SPINLOCKS_H

/********************  HEADERS  *********************/
#include <Common.h>
#include <Locks.h>

/*********************  CLASS  **********************/
class Spinlock
{
	public:
		class TakeLock
		{
			public:
				TakeLock(Spinlock & lock,bool test = true);
				~TakeLock(void);
				void unlock(void);
			private:
				Spinlock * lock;
		};
	public:
		Spinlock(void);
		~Spinlock(void);
		void lock(void);
		void unlock(void);
	private:
		Locks::Spinlock spinlock;
};

/********************  MACRO  ***********************/
#define OPTIONAL_CRITICAL(lock,test) do { Spinlock::TakeLock _local_##lock##__(lock,test);
#define START_CRITICAL(lock) { Spinlock::TakeLock _local_##lock##__(lock);
#define END_CRITICAL }while(0);

#endif //LOCKS
