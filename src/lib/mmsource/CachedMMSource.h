/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef CACHED_MM_SOURCE_H
#define CACHED_MM_SOURCE_H

/********************  HEADERS  *********************/
#include "IAllocator.h"
#include "DoubleLinkList.h"
#include "RegionRegistry.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/********************  MACRO  ***********************/
#define MMSRC_MAX_SIZE (16*1024*1024)
#define MMSRC_THREASHOLD (8*1024*1204)
#define MMSRC_KEEP_RESIDUT false

/*********************  STRUCT  *********************/
class FreeMacroBloc
{
	public:
		FreeMacroBloc(Size totalSize);
		Size getTotalSize(void) const;
		ListElement * getListHandler(void);
		static FreeMacroBloc * getFromListHandler(ListElement * list);
		static FreeMacroBloc * from(RegionSegmentHeader * segment);
		static FreeMacroBloc * setup(void * ptr,Size totalSize);
	private:
		Size totalSize;
		ListElement listElement;
};

/*********************  TYPES  **********************/
typedef DoubleLinkList<FreeMacroBloc> FreeMacroBlocList;

/*********************  CLASS  **********************/
class CachedMMSource : public IMMSource
{
	public:
		CachedMMSource(RegionRegistry * registry = NULL,size_t maxSize = MMSRC_MAX_SIZE,size_t threashold = MMSRC_THREASHOLD,bool keepResidut = MMSRC_KEEP_RESIDUT);
		~CachedMMSource(void);
		virtual RegionSegmentHeader* map ( size_t innerSize, bool* zeroFilled, IChunkManager* manager );
		virtual RegionSegmentHeader* remap ( RegionSegmentHeader* oldSegment, size_t newInnerSize, IChunkManager* manager );
		virtual void unmap ( RegionSegmentHeader* segment );
		void freeAll(void);
	protected:
		RegionSegmentHeader * searchInCache( size_t totalSize, MPCAllocator::IChunkManager* manager );
		FreeMacroBloc* fixReuseSize( MPCAllocator::FreeMacroBloc* segment, size_t totalSize );
	private:
		FreeMacroBlocList freeList;
		size_t currentSize;
		size_t maxSize;
		size_t threashold;
		bool keepResidut;
		Spinlock spinlock;
		RegionRegistry * registry;
};

};

#endif //CACHED_MM_SOURCE_H
