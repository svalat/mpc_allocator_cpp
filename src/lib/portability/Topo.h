/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

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
