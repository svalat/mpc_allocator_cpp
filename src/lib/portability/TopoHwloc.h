/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef PORTABILITY_TOPO_HWLOC_H
#define PORTABILITY_TOPO_HWLOC_H

/********************  HEADERS  *********************/
#include <hwloc.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CLASS  **********************/
class TopoHwloc
{
	public:
		//lifecycle
		TopoHwloc(void);
		~TopoHwloc(void);
		//get info
		int getLevelId(const char * name) const;
		const char * getLevelName(int id) const;
		int getNbNumaEntities(void) const;
		int getNbEntities(int level,int depth) const;
		int getCurrentId(int level, int depth) const;
		int getCurentNumaId(void) const;
		int getCurrentIdFromNUMABinding(void) const;
		int getCurrentIdFromThreadBinding(void) const;
		//load predefined topo
		void loadTopologyFromFile(const char * filename);
	private:
		//some helper methods
		int getAbsDepth(int level,int depth) const;
		int getFirstBitInBitmap(hwloc_bitmap_t bitmap) const;
	private:
		hwloc_topology_t topology;
};

};

#endif //PORTABILITY_TOPO_HWLOC_H
