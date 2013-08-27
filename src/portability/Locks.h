#ifndef PORTABILITY_LOCKS_H
#define PORTABILITY_LOCKS_H

/*********************  TYPES  **********************/
#ifdef LOCKS_Pthread
	#include "LocksPthread.h"
	typedef LocksPthread Locks;
#else
	#error "Unsupported Locks portability mode"
#endif

#endif //PORTABILITY_LOCKS_H
