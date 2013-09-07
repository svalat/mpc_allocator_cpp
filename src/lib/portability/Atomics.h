#ifndef PORTABILITY_ATOMICS_H
#define PORTABILITY_ATOMICS_H

/*********************  TYPES  **********************/
#ifdef ATOMICS_OpenPA
	#include "AtomicsOpenPA.h"
	typedef AtomicsOpenPA Atomics;
#elif defined(ATOMICS_GCC)
	#include "AtomicsGCCIntr.h"
	typedef AtomicsGCC Atomics;
#else
	#error "Unsupported Atomics portability mode"
#endif

#endif //PORTABILITY_ATOMICS_H
