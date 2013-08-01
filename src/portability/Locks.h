#ifndef PORTABILITY_LOCKS_H
#define PORTABILITY_LOCKS_H

/*********************  TYPES  **********************/
#if LOCKS == Pthread
	#include "LocksPthread.h"
	typedef LocksPthread Locks;
#else
	#error "Unsupported Locks portability mode"
#endif

#endif //PORTABILITY_LOCKS_H
