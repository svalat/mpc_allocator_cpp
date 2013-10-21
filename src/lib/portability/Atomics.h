/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef PORTABILITY_ATOMICS_H
#define PORTABILITY_ATOMICS_H

/*********************  TYPES  **********************/
#ifdef ATOMICS_OpenPA
	#include "AtomicsOpenPA.h"
	namespace MPCAllocator { typedef AtomicsOpenPA Atomics; };
#elif defined(ATOMICS_GCC)
	#include "AtomicsGCCIntr.h"
	namespace MPCAllocator { typedef AtomicsGCC Atomics; };
#else
	#error "Unsupported Atomics portability mode"
#endif

#endif //PORTABILITY_ATOMICS_H
