#ifndef LOCKS_H
#define LOCKS_H

/********************  HEADERS  *********************/
#include <pthread.h>

/*********************  CLASS  **********************/
class PThreadLocks
{
	public:
		typedef pthread_spinlock_t Spinlock;
		static void init(Spinlock * spinlock) {pthread_spin_init(spinlock,PTHREAD_PROCESS_PRIVATE);};
		static void lock(Spinlock * spinlock) {pthread_spin_lock(spinlock);};
		static void unlock(Spinlock * spinlock) {pthread_spin_unlock(spinlock);};
		static void destroy(Spinlock * spinlock) {pthread_spin_destroy(spinlock);};
};

/*********************  TYPES  **********************/
typedef PThreadLocks Locks;

#endif //LOCKS_H
