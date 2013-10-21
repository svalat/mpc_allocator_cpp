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

/*********************  CLASS  **********************/
class PaddedChunk
{
	public:
		static PaddedChunk * setup(MediumChunk * parentChunk,Size padding);
		static PaddedChunk * setup(void * ptr,Size padding,Size chunkSize);
		static void * pad(void * ptr,Size padding,Size chunkSize);
		static Size calcPadding( MPCAllocator::MediumChunk* chunk, MPCAllocator::Size align, MPCAllocator::Size requestSize );
		static void * unpad(void * ptr);
		void * getPtr(void);
	private:
		//No public constructor, but be setup by init
		PaddedChunk(void);
	private:
		uint16_t padding;
		uint8_t magick;
};

};

#endif //PADDED_CHUNK_H
