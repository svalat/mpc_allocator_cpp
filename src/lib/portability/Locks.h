/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef PORTABILITY_LOCKS_H
#define PORTABILITY_LOCKS_H

/*********************  TYPES  **********************/
#ifdef LOCKS_Pthread
	#include "LocksPthread.h"
	namespace MPCAllocator{ typedef LocksPthread Locks; };
#else
	#error "Unsupported Locks portability mode"
#endif

#endif //PORTABILITY_LOCKS_H
