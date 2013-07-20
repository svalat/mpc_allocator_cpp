/********************  HEADERS  *********************/
#include "Chunk.h"
#include <cstdlib>
#include <cassert>

/*******************  FUNCTION  *********************/
Chunk* Chunk::setup ( void* ptr, Size size )
{
	//errors
	assert(size >= 2*sizeof(Chunk));
	assert(size % sizeof(Chunk) == 0);
	
	//setup first bloc
	Chunk * res = setup(ptr,NULL,size - sizeof(Chunk));
	
	//setup close block
	setup(res->getNext(),res,0UL);
	
	return res;
}

/*******************  FUNCTION  *********************/
Chunk* Chunk::setup ( void* ptr, Chunk* prev, Chunk* next )
{
	//errors
	assert(next > ptr || next == NULL);
	assert(prev < ptr);

	//generic setup
	Chunk * res = setup(ptr);
	
	//setup links
	res->prev = prev;
	res->next = next;
	
	return res;
}

/*******************  FUNCTION  *********************/
Chunk* Chunk::setup ( void* ptr, Chunk* prev, Size size )
{
	assert(prev < ptr);
	assert(size == 0 || size >= 2*sizeof(Chunk));
	
	Chunk * res = setup(ptr);

	res->prev = prev;
	if (size == 0)
		res->next = NULL;
	else
		res->next = (Chunk*)((Addr)ptr + size);
	
	return res;
}

/*******************  FUNCTION  *********************/
Chunk * Chunk::setup ( void * ptr )
{
	Chunk * res = (Chunk *)ptr;
	assert(res != NULL);
	assert((Addr)ptr % sizeof(Addr) == 0);
	
	#ifndef OPTIMIZED
	res->prev = NULL;
	res->next = NULL;
	res->padding[0] = res->padding[1] = res->padding[2] = 0;
	#endif
	
	res->status = CHUNK_ALLOCATED;
	res->magick = MAGICK_VALUE;
	
	return res;
}

/*******************  FUNCTION  *********************/
void Chunk::check ( void ) const
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
Size Chunk::getInnerSize ( void ) const
{
	if (next == NULL)
		return 0;
	else
		return (next - this - 1) * sizeof(*this);
}

/*******************  FUNCTION  *********************/
Size Chunk::getTotalSize ( void ) const
{
	if (next == NULL)
		return sizeof(Chunk);
	else
		return ((Addr)next - (Addr)this);
}

/*******************  FUNCTION  *********************/
Chunk* Chunk::getNext ( void ) const
{
	return next;
}

/*******************  FUNCTION  *********************/
Chunk* Chunk::getPrev ( void ) const
{
	return prev;
}

/*******************  FUNCTION  *********************/
ChunkStatus Chunk::getStatus ( void ) const
{
	return status;
}

/*******************  FUNCTION  *********************/
void Chunk::setStatus ( ChunkStatus status )
{
	assert(status == CHUNK_FREE || status == CHUNK_ALLOCATED);
	this->status = status;
}

/*******************  FUNCTION  *********************/
Chunk* Chunk::split ( Size size )
{
	//round size to multiple of 8
	size = upToPowOf2(size,BASIC_ALIGN) + sizeof(Chunk);
	
	//check size
	if (size > getTotalSize() - sizeof(Chunk))
		return NULL;
	
	//split
	Chunk * res = setup(addrOffset(this,size),this,next);
	
	//update
	if (this->next != NULL);
		this->next->prev = res;
	this->next = res;
	
	this->check();
	res->check();
	
	return res;
}

/*******************  FUNCTION  *********************/
bool Chunk::isSingle ( void ) const
{
	assert(this != NULL);
	return (this->status == CHUNK_FREE && this->prev == NULL && this->next->next == NULL);
}
