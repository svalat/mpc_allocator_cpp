/********************  HEADERS  *********************/
#include <Debug.h>
#include "SmallChunkRun.h"
#include <cstring>

/*******************  FUNCTION  *********************/
SmallChunkRun::SmallChunkRun ( SmallSize skipedSize , SmallSize splitting)
{
	this->cntAlloc = 0;
	this->skipedSize = upToPowOf2(skipedSize,MACRO_ENTRY_SIZE) / MACRO_ENTRY_SIZE;
	this->splitting = splitting;
	this->bitmapEntries = 0;
	if (splitting > 0)
		setSplitting(splitting);
}

/*******************  FUNCTION  *********************/
MacroEntry * SmallChunkRun::getMacroEntry ( SmallSize id )
{
	//TODO check if compiler use shift
	return data + skipedSize + id / MACRO_ENTRY_BITS;
}

/*******************  FUNCTION  *********************/
void SmallChunkRun::setSplitting ( SmallSize splitting )
{
	//errors
	allocAssert(splitting <= STORAGE_SIZE);
	allocAssume(cntAlloc == 0,"Cannot change the size of non empty SmallChunkRun.");
	
	//get bitmap
	MacroEntry * bitmap = getMacroEntry(0);
	
	//setup size
	this->splitting = splitting;
	
	//calc bitmap entries
	SmallSize bitmapRealEntries = STORAGE_SIZE / splitting;
	this->bitmapEntries = upToPowOf2(bitmapRealEntries,MACRO_ENTRY_BITS);
	
	//calc skiped entries
	SmallSize skipedEntries = getRoundedNbEntries(skipedSize * MACRO_ENTRY_SIZE);
	
	//calc nb entries masked by bitmap storage
	SmallSize bitmapSize = bitmapEntries / 8;
	SmallSize bitmapHiddenEntries = getRoundedNbEntries(bitmapSize);
	
	//clear bitmap size
	for (SmallSize i = 0 ; i < bitmapSize / MACRO_ENTRY_SIZE ; i++)
		bitmap[i] = MACRO_ENTRY_MASK;
	
	//mark skied entries and bitmap part
	for (SmallSize i = 0 ; i < skipedEntries + bitmapRealEntries ; i++)
		setBitStatusZero(i);
	
	//mark last bits to 0
	for (SmallSize i = bitmapRealEntries ; i < bitmapSize ; i++)
		setBitStatusZero(i);
}

/*******************  FUNCTION  *********************/
void SmallChunkRun::free ( void* ptr )
{
	//compute bit position
	//TODO maybe assume
	allocAssert(contain(ptr));

	//calc bit position
	SmallSize bitpos = (Addr)ptr / splitting;

	//check current status
	allocAssert(getBitStatus(bitpos) == false);

	//mark as free
	setBitStatusOne(bitpos);
}

/*******************  FUNCTION  *********************/
bool SmallChunkRun::contain ( void* ptr ) const
{
	return (ptr >= data+skipedSize+bitmapEntries/MACRO_ENTRY_BITS && ptr < addrOffset(data,MACRO_ENTRY_SIZE));
}

/*******************  FUNCTION  *********************/
bool SmallChunkRun::getBitStatus ( SmallSize id )
{
	uint64_t value = *getMacroEntry(id);
	return (value & (1 << (id & MACRO_ENTRY_MASK)));
}

/*******************  FUNCTION  *********************/
void SmallChunkRun::setBitStatusOne ( SmallSize id )
{
	uint64_t * value = getMacroEntry(id);
	*value |= (1 << (id & MACRO_ENTRY_MASK));
}

/*******************  FUNCTION  *********************/
void SmallChunkRun::setBitStatusZero ( SmallSize id )
{
	uint64_t * value = getMacroEntry(id);
	*value &= ~(1 << (id & MACRO_ENTRY_MASK));
}

/*******************  FUNCTION  *********************/
int SmallChunkRun::getRoundedNbEntries ( SmallSize size ) const
{
	SmallSize entries = size / splitting;
	if (entries * splitting != size)
		return entries + 1;
	else
		return entries;
}

/*******************  FUNCTION  *********************/
size_t SmallChunkRun::getInnerSize ( void* ptr )
{
	allocAssert(contain(ptr));
	return splitting;
}

/*******************  FUNCTION  *********************/
size_t SmallChunkRun::getRequestedSize ( void* ptr )
{
	//TODO constant
	return 0;
}

/*******************  FUNCTION  *********************/
size_t SmallChunkRun::getTotalSize ( void* ptr )
{
	allocAssert(contain(ptr));
	return splitting;
}

/*******************  FUNCTION  *********************/
bool SmallChunkRun::isEmpty ( void ) const
{
	return cntAlloc == 0;
}

/*******************  FUNCTION  *********************/
void* SmallChunkRun::realloc ( void* ptr, size_t size )
{
	allocAssume(size <= splitting,"Realloc isn't supported in SmammChunkRun.");
	return ptr;
}

/*******************  FUNCTION  *********************/
void* SmallChunkRun::malloc ( size_t size, size_t align, bool* zeroFilled )
{
	//check size
	allocAssume(size <= splitting,"SmallChunkRun only support allocation smaller the splitting size.");
	
	//search first bit to one (availble free bloc)
	SmallSize macroEntries = bitmapEntries / MACRO_ENTRY_BITS;
	for(SmallSize i = 0 ; i < macroEntries ; i++)
	{
		//if get one bit to 1, it contain free elements
		if (data[i])
		{
			//search the first bit to one
			SmallSize id = fastLog2(data[i]);
			id += i * MACRO_ENTRY_BITS;
			return addrOffset(data,splitting * id);
		}
	}
	
	//didn't not find free memory
	return NULL;
}

/*******************  FUNCTION  *********************/
SmallSize SmallChunkRun::getSplitting ( void ) const
{
	return splitting;
}
