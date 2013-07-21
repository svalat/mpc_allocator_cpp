#ifndef SPINLOCKS_H
#define SPINLOCKS_H

/********************  HEADERS  *********************/
#include <pthread.h>

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
		pthread_spinlock_t spinlock;
};

#define OPTINAL_CRITICAL(lock,test) { Spinlock::TakeLock _local_##lock##__(lock,test);
#define END_CRITICAL }while(1);

#endif //LOCKS
