#ifndef LOCKS_PTHREAD_H
#define LOCKS_PTHREAD_H

/********************  HEADERS  *********************/
#include <pthread.h>

/*********************  CLASS  **********************/
class LocksPthread
{
	public:
		typedef pthread_spinlock_t Spinlock;
		static void init(Spinlock * spinlock) {pthread_spin_init(spinlock,PTHREAD_PROCESS_PRIVATE);};
		static void lock(Spinlock * spinlock) {pthread_spin_lock(spinlock);};
		static void unlock(Spinlock * spinlock) {pthread_spin_unlock(spinlock);};
		static void destroy(Spinlock * spinlock) {pthread_spin_destroy(spinlock);};
};

#endif //LOCKS_PTHREAD_H
