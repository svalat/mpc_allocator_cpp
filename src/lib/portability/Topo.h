#ifndef PORTABILITY_TOPO_H
#define PORTABILITY_TOPO_H

/*********************  TYPES  **********************/
#ifdef TOPO_Hwloc
	#include "TopoHwloc.h"
	namespace MPCAllocator { typedef TopoHwloc Topo; };
#else
	#error "Unsupported topo portability mode"
#endif

#endif //PORTABILITY_TOPO_H
