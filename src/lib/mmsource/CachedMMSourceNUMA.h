/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef CACHED_MM_SOURCE_NUMA_H
#define CACHED_MM_SOURCE_NUMA_H

/********************  HEADERS  *********************/
#include "CachedMMSource.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CLASS  **********************/
class CachedMMSourceNUMA : public CachedMMSource
{
	public:
		CachedMMSourceNUMA(int nodeId, RegionRegistry * registry = NULL,size_t maxSize = MMSRC_MAX_SIZE,size_t threashold = MMSRC_THREASHOLD,bool keepResidut = MMSRC_KEEP_RESIDUT);
		virtual RegionSegmentHeader* map ( size_t innerSize, bool* zeroFilled, IChunkManager* manager );
	private:
		int nodeId;
};

};

#endif //CACHED_MM_SOURCE_NUMA_H
