/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef LOCKS_PTHREAD_H
#define LOCKS_PTHREAD_H

/********************  HEADERS  *********************/
#include <pthread.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CONSTS  *********************/
#define LOCKS_INIT_MUTEX PTHREAD_MUTEX_INITIALIZER

/*********************  CLASS  **********************/
class LocksPthread
{
	public:
		typedef pthread_spinlock_t Spinlock;
		typedef pthread_mutex_t StaticInitLock;
		//spinlocks
		static void init(Spinlock & spinlock) {pthread_spin_init(&spinlock,PTHREAD_PROCESS_PRIVATE);};
		static void lock(Spinlock & spinlock) {pthread_spin_lock(&spinlock);};
		static void unlock(Spinlock & spinlock) {pthread_spin_unlock(&spinlock);};
		static void destroy(Spinlock & spinlock) {pthread_spin_destroy(&spinlock);};
		//init locks (to be setup with static value at boot time)
		static void lock(StaticInitLock & lock) {pthread_mutex_lock(&lock);};
		static void unlock(StaticInitLock & lock) {pthread_mutex_unlock(&lock);};
};

};

#endif //LOCKS_PTHREAD_H
