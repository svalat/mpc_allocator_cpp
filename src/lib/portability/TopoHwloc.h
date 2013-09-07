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
		int getNbEntities(int level,int depth) const;
		int getCurrentId(int level, int depth) const;
		//load predefined topo
		void loadTopologyFromFile(const char * filename);
	private:
		//some helper methods
		int getAbsDepth(int level,int depth) const;
	private:
		hwloc_topology_t topology;
};

};

#endif //PORTABILITY_TOPO_HWLOC_H
