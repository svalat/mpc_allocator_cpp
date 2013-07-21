/********************  HEADERS  *********************/
#include <cstring>
#include "MediumAllocator.h"

/*******************  FUNCTION  *********************/
MediumAllocator::MediumAllocator ( bool useLocks, IAllocator* memorySource )
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
	assert(this != NULL);
	assert(size >= BASIC_ALIGN);
	assert(align == BASIC_ALIGN);//TODO need support
	assert(align >= BASIC_ALIGN);
	
	//trivial
	if ( size == 0)
		return NULL;
	
	//zero
	if (zeroFilled != NULL)
		zero = *zeroFilled;
	
	//align size
	size = upToPowOf2( size,BASIC_ALIGN);
	
	//take lock for the current function
	OPTINAL_CRITICAL(spinlock,useLocks);
		//try to get memory
		chunk = pool.findChunk( size );
		if (chunk == NULL)
			chunk = refill(size,zeroFilled);

		//error out of memory (unlocking is managed by TakeLock destructor)
		if (chunk == NULL)
			return NULL;
		
		//try to split
		MediumChunk * residut = split(chunk);
		if (residut != NULL)
			pool.insert(residut,CHUNK_INSERT_LIFO);
		assert(chunk->getInnerSize() >= size);
		//assume_m(sctk_alloc_get_size(vchunk) >= sctk_alloc_calc_chunk_size(size), "Size error in chunk spliting function.");
	END_CRITICAL
	
	//setup zero
	if (zeroFilled != NULL)
		*zeroFilled = zero;

	//ok this is good
	return chunk->getPtr();
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumAllocator::refill ( size_t size, bool * zeroFilled )
{
	//vars
	void * ptr;

	//errors
	assert(size > 0);
	
	//trivial
	if (memorySource == NULL)
		return NULL;
	
	//request mem
	ptr = memorySource->malloc(size,BASIC_ALIGN,zeroFilled );
	
	//errors
	if (ptr == NULL)
		return NULL;
	
	//add to registry TODO
	
	//build chunk
	Size innerSize = memorySource->getInnerSize(ptr);
	MediumChunk * chunk = MediumChunk::setup(ptr,size);
	
	//ok return it
	return chunk;
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
	
	//take lock for the current function
	OPTINAL_CRITICAL(spinlock,useLocks);
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
		assert(chunk->isSingle());
		memorySource->free(chunk);
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
		return chunk->getInnerSize();
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
	assert(chunk != NULL);
	//TODO assume
	Size oldSize = chunk->getInnerSize();
	Size delta = size - oldSize;
	
	//if can resuse old one without resize
	if (oldSize >= size && delta <= REALLOC_THREASHOLD)
		return ptr;
	
	//ok do alloc/copy/free
	void * new_ptr = this->malloc(size);
	memcpy(new_ptr,ptr,max(size,oldSize));
	free(ptr);
	
	return new_ptr;
}
