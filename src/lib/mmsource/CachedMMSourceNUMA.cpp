/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include "CachedMMSourceNUMA.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
MPCAllocator::CachedMMSourceNUMA::CachedMMSourceNUMA(int nodeId, RegionRegistry* registry, size_t maxSize, size_t threashold, bool keepResidut)
{
	this->nodeId = nodeId;
}

/*******************  FUNCTION  *********************/
RegionSegmentHeader* MPCAllocator::CachedMMSourceNUMA::map(size_t innerSize, bool* zeroFilled, IChunkManager* manager)
{
	//mmap
	RegionSegmentHeader * segment = MPCAllocator::CachedMMSource::map(innerSize, zeroFilled, manager);
	
	//force finding
	
	//return
	return segment;
}


};
