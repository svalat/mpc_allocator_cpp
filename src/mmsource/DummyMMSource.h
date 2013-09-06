#ifndef DUMMY_MM_SOURCE_H
#define DUMMY_MM_SOURCE_H

/********************  HEADERS  *********************/
#include <IAllocator.h>
#include <RegionRegistry.h>

/*******************  FUNCTION  *********************/
class DummyMMSource : public IMMSource
{
	public:
		DummyMMSource(RegionRegistry * registry = NULL);
		virtual RegionSegmentHeader* map ( size_t innerSize, bool* zeroFilled, IChunkManager * manager = NULL );
		virtual RegionSegmentHeader* remap ( RegionSegmentHeader* oldSegment, size_t newInnerSize, IChunkManager * manager = NULL );
		virtual void unmap ( RegionSegmentHeader* segment);
	private:
		RegionRegistry * registry;
};

#endif //DUMMY_MM_SOURCE_H
