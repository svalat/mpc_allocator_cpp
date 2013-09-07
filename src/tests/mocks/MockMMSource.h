#ifndef MOCK_MM_SOURCE_H
#define MOCK_MM_SOURCE_H

/********************  HEADERS  *********************/
#include <IAllocator.h>
#include <gmock/gmock.h>

/**********************  USING  *********************/
using namespace MPCAllocator;

/*********************  CLASS  **********************/
class MockMMSource : public IMMSource {
	public:
		MOCK_METHOD3(map,RegionSegmentHeader * (size_t innerSize,bool* zeroFilled,IChunkManager* manager));
		MOCK_METHOD3(remap,RegionSegmentHeader * (RegionSegmentHeader * oldSegment,size_t innerSize,IChunkManager* manager));
		MOCK_METHOD1(unmap,void (RegionSegmentHeader * segment));
};

#endif //MOCK_MM_SOURCE_H
