/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include "TopoHwloc.h"
#include "Debug.h"
#include <hwloc.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
TopoHwloc::TopoHwloc ( void )
{
	//vars
	int status;
	
	//init hwloc
	status = hwloc_topology_init(&topology);
	allocAssume(status == 0,"Failed to init hwloc.");
	
	//load current topology
	status = hwloc_topology_load(topology);
	allocAssume(status == 0,"Failed to load current topology with hwloc.");
}

/*******************  FUNCTION  *********************/
TopoHwloc::~TopoHwloc ( void )
{
	hwloc_topology_destroy(topology);
}

/*******************  FUNCTION  *********************/
void TopoHwloc::loadTopologyFromFile ( const char* filename )
{
	//vars
	int status;

	//destroy the old topology
	hwloc_topology_destroy(topology);
	
	//load the new def
	status = hwloc_topology_set_xml(topology,filename);
	allocAssume(status == 0,"Failed to load topological information from file ... in hwloc.");//,filename);
	
	//load current topology
	status = hwloc_topology_load(topology);
	allocAssume(status == 0,"Failed to apply topology from ... with hwloc.");//,filename);
}

/*******************  FUNCTION  *********************/
int TopoHwloc::getLevelId ( const char* name ) const
{
	//convert
	hwloc_obj_type_t res = hwloc_obj_type_of_string(name);
	allocAssume(res != -1,"Invalid topological level name ... for hwloc.");//name
	
	return res;
}

/*******************  FUNCTION  *********************/
const char* TopoHwloc::getLevelName ( int id ) const
{
	const char * res = hwloc_obj_type_string((hwloc_obj_type_t)id);
	allocAssume(res != NULL,"Failed to conver topological level id ... from hwloc.");//id
	
	return res;
}

/*******************  FUNCTION  *********************/
int TopoHwloc::getNbEntities ( int level, int depth ) const
{
	//get real depth
	int absDepth = getAbsDepth(level,depth);
	allocAssume(depth != -1,"Cannot find the depth for the requested level, depth couple in hwloc.");//level,depth
	
	//get number of objs
	int res = hwloc_get_nbobjs_by_depth(topology, absDepth);
	allocAssume(res > 0, "Invalid nbobjs_by_depth in hwloc for absolute depth ....");//absDepth
	return res;
}

/*******************  FUNCTION  *********************/
int TopoHwloc::getAbsDepth ( int level, int depth ) const
{
	//vars
	int curDepth = 0;
	
	//get max depth
	int topodepth = hwloc_topology_get_depth(topology);
	
	for (int i = 0 ; i < topodepth ; i++)
	{
		//get current type
		hwloc_obj_type_t type = hwloc_get_depth_type(topology,i);
		if (type == level)
		{
			curDepth++;
			if (curDepth == depth)
				return i;
		}
	}
	
	//error
	allocFatal("Fail to find the depth of requested level levelType=%d, depth=%d in hwloc.",level,depth);
	return -1;
}

/*******************  FUNCTION  *********************/
int TopoHwloc::getCurrentId ( int level, int depth ) const
{
	//vars
	int res = -1;

	//get absolute depth
	int absDepth = getAbsDepth(level,depth);
	
	//search current thread binding
	hwloc_cpuset_t cpuset = hwloc_bitmap_alloc();
	int status = hwloc_get_cpubind (topology, cpuset, 0);
	allocAssume(status >= 0,"Failed to get the current thread CPU binding with hwloc.");
	
	//count overlap over current level
	int cnt = hwloc_get_nbobjs_inside_cpuset_by_depth(topology,cpuset,absDepth);
	
	//if get only one return its ID
	if (cnt == 1)
	{
		hwloc_obj_t obj = hwloc_get_obj_inside_cpuset_by_depth(topology,cpuset,absDepth,0);
		//TODO maybe their is an accessor for this
		res = obj->logical_index;
	}
	
	//clean memory
	hwloc_bitmap_free(cpuset);
	
	return res;
}

};
