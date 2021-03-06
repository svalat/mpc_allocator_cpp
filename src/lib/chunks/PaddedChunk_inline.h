/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef PADDED_CHUNK_INLINE_H
#define PADDED_CHUNK_INLINE_H

/********************  HEADERS  *********************/
#include "Debug.h"
#include "RegionRegistry.h"
#include "MediumChunk.h"
#include "PaddedChunk.h"

/********************  MACRO  ***********************/
#define PADDED_CHUNK_MAGICK 0x42

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
inline PaddedChunk * PaddedChunk::setup (MediumChunk* parentChunk, MPCAllocator::Size padding )
{
	//errors
	allocAssert(parentChunk != NULL);
	return setup(parentChunk->getPtr(),padding,parentChunk->getInnerSize());
}

/*******************  FUNCTION  *********************/
inline PaddedChunk * PaddedChunk::setup (RegionSegmentHeader * segment, MPCAllocator::Size padding )
{
	//errors
	allocAssert(segment != NULL);
	return setup(segment->getPtr(),padding,segment->getInnerSize());
}

/*******************  FUNCTION  *********************/
inline PaddedChunk* PaddedChunk::setup ( void* ptr, Size padding, Size chunkSize )
{
	//errors
	assert(ptr != NULL);
	assert(chunkSize >= padding);
	allocAssert(padding >= sizeof(PaddedChunk));
	allocAssert(padding < (1UL<<(sizeof(PaddedChunk::padding)*8)));
	
	//compute base address
	PaddedChunk * paddedChunk = (PaddedChunk*)addrOffset(ptr,padding) - 1;
	
	//setup data
	paddedChunk->magick = PADDED_CHUNK_MAGICK;
	paddedChunk->padding = padding;

	//check
	allocAssert(addrDelta(paddedChunk->getPtr(),ptr) == (long int)padding);
	
	return paddedChunk;
}

/*******************  FUNCTION  *********************/
inline void* PaddedChunk::getPtr ( void )
{
	if (this == NULL)
		return NULL;
	else
		return this+1;
}

/*******************  FUNCTION  *********************/
inline Size PaddedChunk::calcPadding ( MediumChunk * chunk, Size align, Size requestSize )
{
	//errors
	allocAssert(chunk != NULL);
	
	//calc current align
	Size delta = (Addr)chunk->getPtr() % align;
	if (delta != 0)
	{
		delta = align - delta;
		allocAssert(delta >= sizeof(PaddedChunk));
		/*if (delta < sizeof(PaddedChunk))
			delta += align;*/
	}
	
	//check size
	allocAssert(chunk->getInnerSize() >= delta + requestSize);
	
	return delta;	
}

/*******************  FUNCTION  *********************/
inline Size PaddedChunk::calcPadding ( RegionSegmentHeader * segment, Size align, Size requestSize )
{
	//errors
	allocAssert(segment != NULL);
	
	//calc current align
	Size delta = (Addr)segment->getPtr() % align;
	if (delta != 0)
	{
		delta = align - delta;
		allocAssert(delta >= sizeof(PaddedChunk));
		/*if (delta < sizeof(PaddedChunk))
			delta += align;*/
	}
	
	//check size
	allocAssert(segment->getInnerSize() >= delta + requestSize);
	
	return delta;	
}

/*******************  FUNCTION  *********************/
inline void* PaddedChunk::pad ( void* ptr, Size padding, Size chunkSize )
{
	PaddedChunk * padded = PaddedChunk::setup(ptr,padding,chunkSize);
	return padded->getPtr();
}

/*******************  FUNCTION  *********************/
inline void * PaddedChunk::unpad( void* ptr )
{
	//trivial
	if (ptr == NULL)
		return NULL;

	PaddedChunk * padded = (PaddedChunk*)ptr - 1;
	if (padded->magick == PADDED_CHUNK_MAGICK)
		return (char*)ptr - padded->padding;
	else
		return ptr;
}

};

#endif //PADDED_CHUNK_INLINE_H
