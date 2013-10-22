/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef PADDED_CHUNK_H
#define PADDED_CHUNK_H

/********************  HEADERS  *********************/
#include "Common.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{
	
/*********************  TYPES  **********************/
struct MediumChunk;
class RegionSegmentHeader;

/*********************  CLASS  **********************/
class PaddedChunk
{
	public:
		static inline PaddedChunk * setup(RegionSegmentHeader * parentChunk,Size padding);
		static inline PaddedChunk * setup(MediumChunk * parentChunk,Size padding);
		static inline PaddedChunk * setup(void * ptr,Size padding,Size chunkSize);
		static inline void * pad(void * ptr,Size padding,Size chunkSize);
		static inline Size calcPadding( MediumChunk* chunk, Size align, Size requestSize );
		static inline Size calcPadding( RegionSegmentHeader* chunk, Size align, Size requestSize );
		static inline void * unpad(void * ptr);
		inline void * getPtr(void);
	private:
		//No public constructor, but be setup by init
		PaddedChunk(void);
	private:
		uint16_t padding;
		uint8_t magick;
};

};

/********************  HEADERS  *********************/
#include "PaddedChunk_inline.h"

#endif //PADDED_CHUNK_H
