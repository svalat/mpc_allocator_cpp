#ifndef CHUNK_H
#define CHUNK_H

/********************  HEADERS  *********************/
#include "Common.h"

/********************  ENUM  ************************/
enum ChunkStatus
{
	CHUNK_FREE,
	CHUNK_ALLOCATED
};

/*********************  CLASS  **********************/
class Chunk
{
	public:
		static Chunk * setup(void * ptr,Size size);//checked
		static Chunk * setup(void * ptr,Chunk * prev,Size size);//checked
		static Chunk * setup(void * ptr,Chunk * prev,Chunk * next);
		Size getTotalSize(void) const;//checked
		Size getInnerSize(void) const;//checked
		void check(void) const;//checked
		Chunk * getNext(void) const;//checked
		Chunk * getPrev(void) const;//checked
		ChunkStatus getStatus(void) const;//checked
		void setStatus(ChunkStatus status);//checked
		Chunk * split(Size size);//checked
		bool isSingle(void) const;//checked
	private:
		Chunk(void);
		Chunk ( const Chunk& other );
		Chunk& operator= ( const Chunk& other );
		static Chunk * setup(void * ptr);
	private:
		Chunk * next;
		Chunk * prev;
		ChunkStatus status;
		char padding[3];
		uint32_t magick;
		
};

#endif // CHUNK_H
