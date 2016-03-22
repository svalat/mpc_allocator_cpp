/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdio>
#include "PosixAllocatorNUMA.h"
#include "CachedMMSourceNUMA.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
PosixAllocatorNUMA::PosixAllocatorNUMA(bool useStrict)
	:internalAllocator(&registry,&mmSource)
{
}

/*******************  FUNCTION  *********************/
void PosixAllocatorNUMA::postInit(void)
{
    //call parent in case
	MPCAllocator::PosixAllocator::postInit();
	
	//setup
	this->useStrict = useStrict;
	
	//setup internall allocator by default to handle hwloc allocations
	this->setCurrentAllocator(&this->internalAllocator);
	
	//load the topology
	this->topo = new Topo();
	this->numaTopoId = this->topo->getLevelId("Node");
	fprintf(stderr,"==> TopoId : %d == %d\n",this->numaTopoId,HWLOC_OBJ_NODE);
	
	//get number of element of topo level
	//TODO patch to support multple NUMA levels, here we handle only the 1st one
	//this->numaTopoCnt = this->topo->getNbEntities(numaTopoId,0);
	this->numaTopoCnt = this->topo->getNbNumaEntities();
	fprintf(stderr,"==> TopoCnt : %d\n",this->numaTopoId);
	
	//allocate arrays
	if (this->numaTopoCnt > 0)
	{
		this->numaStrictMMSources = new IMMSource*[this->numaTopoCnt];
		this->numaMMSources = new IMMSource*[this->numaTopoCnt];
		
		//create non strict & strict numa sources
		for (int i = 0 ; i < this->numaTopoCnt ; i++)
		{
			this->numaStrictMMSources[i] = new CachedMMSourceNUMA(i,&registry);
			this->numaMMSources[i] = new CachedMMSource(&registry);
		}
	} else {
		this->numaStrictMMSources = NULL;
		this->numaMMSources = NULL;
	}
	
	//can reset
	this->resetDefaultAllocator();
}

/*******************  FUNCTION  *********************/
IMMSource* PosixAllocatorNUMA::getMMSource(void)
{
	//if not NUMA, trivial
	if(this->numaTopoCnt == 0)
		return &mmSource;
	
	//get current node
	//TODO patch to support multple NUMA levels, here we handle only the 1st one
	int id = topo->getCurrentId(numaTopoId,0);
	allocAssume(id >= -1 && id < numaTopoCnt,"Invalid NUMA ID");
	
	//select, if no binding we used the shared unsafe one
	if (id == -1)
		return &mmSource;
	else if (useStrict)
		return numaStrictMMSources[id];
	else
		return numaMMSources[id];
}

};
