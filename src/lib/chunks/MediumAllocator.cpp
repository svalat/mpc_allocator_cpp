/********************  HEADERS  *********************/
#include <cstring>
#include "Debug.h"
#include "MediumAllocator.h"
#include "RegionRegistry.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
MediumAllocator::MediumAllocator ( bool useLocks, IMMSource* memorySource )
{
	this->memorySource = memorySource;
	this->useLocks = useLocks;
}

/*******************  FUNCTION  *********************/
void * MediumAllocator::malloc ( size_t size, size_t align, bool * zeroFilled )
{
	bool zero = false;
	MediumChunk * chunk = NULL;

	//errors
	allocAssert(this != NULL);
	allocAssert(align == BASIC_ALIGN);//TODO need support
	allocAssert(align >= BASIC_ALIGN);
	
	//trivial
	if ( size == 0)
		return NULL;
	else if (size < MEDIUM_MIN_INNER_SIZE)
		size = MEDIUM_MIN_INNER_SIZE;
	
	//zero
	if (zeroFilled != NULL)
		zero = *zeroFilled;
	
	//align size
	size = upToPowOf2( size,BASIC_ALIGN);
	
	//take lock for the current function
	OPTIONAL_CRITICAL(spinlock,useLocks);
		//try to get memory
		chunk = pool.findChunk( size );
		if (chunk == NULL)
			chunk = refill(size,zeroFilled);

		//error out of memory (unlocking is managed by TakeLock destructor)
		if (chunk == NULL)
			return NULL;
		
		//try to split
		MediumChunk * residut = split(chunk,size);
		allocAssert(chunk->getInnerSize() >= size);
		if (residut != NULL)
			pool.insert(residut,CHUNK_INSERT_LIFO);
		//assume_m(sctk_alloc_get_size(vchunk) >= sctk_alloc_calc_chunk_size(size), "Size error in chunk spliting function.");
	END_CRITICAL
	
	//setup zero
	if (zeroFilled != NULL)
		*zeroFilled = zero;

	//ok this is good
	return chunk->getPtr();
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumAllocator::split ( MediumChunk* chunk,size_t innerSize )
{
	//trivial
	if (chunk == NULL)
		return NULL;
	
	//align request size
	innerSize = upToPowOf2(innerSize,MEDIUM_MIN_INNER_SIZE);
	
	//get avail size
	Size availSize = chunk->getInnerSize();
	
	//check minimal size
	if (availSize - innerSize <= MEDIUM_MIN_INNER_SIZE + sizeof(MediumChunk))
		return NULL;
	
	//resize
	return chunk->split(innerSize);
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumAllocator::refill ( size_t size, bool * zeroFilled )
{
	//errors
	allocAssert(size > 0);
	
	//trivial
	if (memorySource == NULL)
		return NULL;
	
	//request mem
	RegionSegmentHeader * segment = memorySource->map(size,zeroFilled,this);
	if (segment == NULL)
		return NULL;
	allocAssert(segment->getInnerSize() >= size);
	
	//get inner segment
	void * ptr = segment->getPtr();
	allocAssert(segment == RegionSegmentHeader::getSegment(ptr));
	
	//build chunk
	Size innerSize = segment->getInnerSize();
	MediumChunk * chunk = MediumChunk::setup(ptr,innerSize);
	allocAssert(segment == RegionSegmentHeader::getSegment(chunk));
	
	//ok return it
	return chunk;
}

/*******************  FUNCTION  *********************/
void MediumAllocator::fill ( void* ptr, size_t size, RegionRegistry * registry )
{
	//vars
	MediumChunk * chunk = NULL;

	//errors
	allocAssert(ptr != NULL);
	allocAssert(size > 0);
	
	//trivial
	if (ptr == NULL || size == 0)
		return;
	
	//if need register, create macro bloc
	if (registry != NULL)
	{
		RegionSegmentHeader * segment = registry->setEntry(ptr,size,this);
		chunk = MediumChunk::setup(segment->getPtr(),segment->getInnerSize());
	} else {
		//create chunk
		chunk = MediumChunk::setup(ptr,size);
	}
	
	//put in free list
	pool.insert(chunk,CHUNK_INSERT_FIFO);
}

/*******************  FUNCTION  *********************/
void MediumAllocator::free ( void* ptr )
{
	//trivial
	if (ptr == NULL)
		return;
	
	//get chunk
	MediumChunk * chunk = MediumChunk::getChunkSafe(ptr);
	if(chunk == NULL)
		return;
	
	//check status
	if (chunk->getStatus() == CHUNK_FREE)
	{
		allocCondWarning(ALLOC_DO_WARNING,"Double free, ignoring the request.");
		return;
	}
	
	//take lock for the current function
	OPTIONAL_CRITICAL(spinlock,useLocks);
		//try merge
		chunk = pool.merge(chunk);
		
		//if whe have a source, we may try to check if we can clear the bloc
		//we didn't do it here to avoid to take time in critical section
		//as this actions didn't require the local lock
		if (memorySource == NULL || chunk->isSingle() == false)
		{
			pool.insert(chunk,CHUNK_INSERT_FIFO);
			chunk = NULL;
		}
	END_CRITICAL
	
	//if need final free to mm source
	if (memorySource != NULL && chunk != NULL)
	{
		allocAssert(chunk->isSingle());
		memorySource->unmap(RegionSegmentHeader::getSegment(chunk));
	}
}

/*******************  FUNCTION  *********************/
size_t MediumAllocator::getInnerSize ( void* ptr )
{
	//trivial
	if (ptr == NULL)
		return 0;
	
	MediumChunk * chunk = MediumChunk::getChunkSafe(ptr);
	if (chunk == NULL)	
		return 0;
	else
		return chunk->getInnerSize();
}

/*******************  FUNCTION  *********************/
size_t MediumAllocator::getTotalSize ( void* ptr )
{
	//trivial
	if (ptr == NULL)
		return 0;
	
	MediumChunk * chunk = MediumChunk::getChunkSafe(ptr);
	if (chunk == NULL)	
		return 0;
	else
		return chunk->getTotalSize();
}

/*******************  FUNCTION  *********************/
size_t MediumAllocator::getRequestedSize ( void* ptr )
{
	return UNSUPPORTED;
}

/*******************  FUNCTION  *********************/
void* MediumAllocator::realloc ( void* ptr, size_t size )
{
	//trivial
	if (ptr == NULL && size == 0)
	{
		return NULL;
	} else if (ptr == NULL) {
		return this->malloc(size);
	} else if (size == 0) {
		this->free(ptr);
		return NULL;
	}
	
	//get old size
	MediumChunk * chunk = MediumChunk::getChunkSafe(ptr);
	allocAssert(chunk != NULL);
	//TODO assume
	Size oldSize = chunk->getInnerSize();
	Size delta = oldSize - size;
	
	//if can resuse old one without resize
	if (oldSize >= size && delta <= REALLOC_THREASHOLD)
		return ptr;
	
	//check if can realloc the next one
	//TODO maybe find a way to avoid to retake the lock for next malloc call
	OPTIONAL_CRITICAL(spinlock,useLocks);
		MediumChunk * merged;
		if (size > oldSize)
			merged = pool.tryMergeForSize(chunk,size);
		else
			merged = chunk;
		if (merged != NULL)
		{
			//check
			allocAssert(merged == chunk);
			allocAssert(merged->getInnerSize() >= size);
	
			//check for split
			MediumChunk * residut = split(merged,size);
			allocAssert(merged->getInnerSize() >= size);
			if (residut != NULL)
				pool.insert(residut,CHUNK_INSERT_LIFO);
			
			//ok return, the lock is auto removed by TakeLock destructor
			return merged->getPtr();
		}
	END_CRITICAL
	
	//ok do alloc/copy/free
	void * new_ptr = this->malloc(size);
	if (new_ptr != NULL)
		memcpy(new_ptr,ptr,max(size,oldSize));
	free(ptr);
	
	return new_ptr;
}

};
