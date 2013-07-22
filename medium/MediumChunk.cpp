/********************  HEADERS  *********************/
#include "MediumChunk.h"
#include <cstdlib>
#include <cassert>

/*******************  FUNCTION  *********************/
MediumChunk* MediumChunk::setup ( void* ptr, Size totalSize )
{
	//errors
	assert( totalSize >= sizeof(MediumChunk) + BASIC_ALIGN);
	assert( totalSize % BASIC_ALIGN == 0);
	
	//setup first bloc
	MediumChunk * res = setup(ptr,NULL,totalSize - sizeof(MediumChunk));
	
	//setup close block
	setup(res->getNext(),res,0UL);
	
	return res;
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumChunk::setup ( void* ptr, MediumChunk* prev, MediumChunk* next )
{
	//errors
	assert(next > ptr || next == NULL);
	assert(prev < ptr);

	//generic setup
	MediumChunk * res = setup(ptr);
	
	//setup links
	res->prev = prev;
	res->next = next;
	
	return res;
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumChunk::setup ( void* ptr, MediumChunk* prev, Size totalSize )
{
	assert(prev < ptr);
	assert( totalSize == 0 || totalSize >= sizeof(MediumChunk) + BASIC_ALIGN);
	
	MediumChunk * res = setup(ptr);

	res->prev = prev;
	if ( totalSize == 0)
		res->next = NULL;
	else
		res->next = (MediumChunk*)((Addr)ptr + totalSize );
	
	return res;
}

/*******************  FUNCTION  *********************/
MediumChunk * MediumChunk::setup ( void * ptr )
{
	MediumChunk * res = (MediumChunk *)ptr;
	assert(res != NULL);
	assert((Addr)ptr % sizeof(Addr) == 0);
	
	#ifndef OPTIMIZED
	res->prev = NULL;
	res->next = NULL;
	#endif
	
	res->status = CHUNK_ALLOCATED;
	res->magick = MAGICK_VALUE;
	
	return res;
}

/*******************  FUNCTION  *********************/
void MediumChunk::check ( void ) const
{
	assert(this != NULL);
	assert(status == CHUNK_FREE || status == CHUNK_ALLOCATED);
	assert(magick == MAGICK_VALUE);
	if (prev != NULL)
		assert(prev->next == this);
	if (next != NULL)
		assert(next->prev == this);
}

/*******************  FUNCTION  *********************/
Size MediumChunk::getInnerSize ( void ) const
{
	if (next == NULL)
		return 0;
	else
		return (next - this - 1) * sizeof(*this);
}

/*******************  FUNCTION  *********************/
Size MediumChunk::getTotalSize ( void ) const
{
	if (next == NULL)
		return sizeof(MediumChunk);
	else
		return ((Addr)next - (Addr)this);
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumChunk::getNext ( void ) const
{
	return next;
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumChunk::getPrev ( void ) const
{
	return prev;
}

/*******************  FUNCTION  *********************/
ChunkStatus MediumChunk::getStatus ( void ) const
{
	return (ChunkStatus)status;
}

/*******************  FUNCTION  *********************/
void MediumChunk::setStatus ( ChunkStatus status )
{
	assert(status == CHUNK_FREE || status == CHUNK_ALLOCATED);
	this->status = status;
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumChunk::split ( Size innerSize )
{
	//round size to multiple of 8
	Size totalSize = upToPowOf2(innerSize,BASIC_ALIGN) + sizeof(MediumChunk);
	
	//check size
	if (totalSize > getInnerSize())
		return NULL;
	
	//split
	MediumChunk * chunk = setup(addrOffset(this,totalSize),this,next);
	
	//update
	if (this->next != NULL);
		this->next->prev = chunk;
	this->next = chunk;
	
	this->check();
	chunk->check();
	
	return chunk;
}

/*******************  FUNCTION  *********************/
bool MediumChunk::isSingle ( void ) const
{
	assert(this != NULL);
	//this->status == CHUNK_FREE && 
	return (this->prev == NULL && this->next->next == NULL);
}

/*******************  FUNCTION  *********************/
void* MediumChunk::getPtr ( void )
{
	if (this == NULL)
		return NULL;
	else
		return (this+1);
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumChunk::getChunk ( void* ptr )
{
	if (ptr == NULL)
		return NULL;
	
	MediumChunk * chunk = (MediumChunk*)ptr - 1;
	chunk->check();
	
	return chunk;
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumChunk::getChunkSafe ( void* ptr )
{
	if (ptr == NULL)
		return NULL;
	
	MediumChunk * chunk = (MediumChunk*)ptr - 1;
	chunk->check();//TODO implementer strict (work without asserts) check and return NULL if not ok
	
	return chunk;
}

/*******************  FUNCTION  *********************/
void MediumChunk::merge ( MediumChunk* last )
{
	MediumChunk * first = this;
	
	//errors
	first->check();
	last->check();
	assert(last >= first);

	if (first == last)
		return;
	
	//compute size
	//Size totalSize = last->next - first;

	if (last->next != NULL)
		last->next->prev = first;
	
	first->next = last->next;
}
