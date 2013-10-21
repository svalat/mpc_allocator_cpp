/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <cstring>
#include <cstddef>
#include <Debug.h>
#include "SmallChunkRun.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
SmallChunkRun::SmallChunkRun ( SmallSize skipedSize , SmallSize splitting,SmallRunContainer * container)
{
	this->cntAlloc = 0;
	this->skipedSize = upToPowOf2(skipedSize,MACRO_ENTRY_SIZE) / MACRO_ENTRY_SIZE;
	this->splitting = splitting;
	this->bitmapEntries = 0;
	this->container = container;
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
SmallRunContainer* SmallChunkRun::getContainer ( void )
{
	return container;
}

/*******************  FUNCTION  *********************/
const MacroEntry * SmallChunkRun::getMacroEntry ( SmallSize id ) const
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
	
	//trivial
	if (splitting == 0)
	{
		allocAssert(this->splitting > 0);
		this->splitting = 0;
		return;
	}
	
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
	
	//check
	allocAssert(bitmapEntries > bitmapHiddenEntries - skipedEntries );
	
	//clear bitmap with 1 (all free)
	for (SmallSize i = 0 ; i < bitmapSize / MACRO_ENTRY_SIZE ; i++)
		bitmap[i] = -1UL;
	
	//mark skiped entries and bitmap part
	for (SmallSize i = 0 ; i < skipedEntries + bitmapHiddenEntries ; i++)
		setBitStatusZero(i);
	
	//mark last bits to 0
	for (SmallSize i = bitmapRealEntries ; i < bitmapEntries ; i++)
		setBitStatusZero(i);
}

/*******************  FUNCTION  *********************/
void SmallChunkRun::free ( void* ptr )
{
	//compute bit position
	//TODO maybe assume
	allocAssert(contain(ptr));

	//calc bit position
	SmallSize bitpos = addrDelta(ptr,data) / splitting;

	//check current status
	allocAssert(getBitStatus(bitpos) == false);

	//mark as free
	setBitStatusOne(bitpos);
	
	//update counter
	cntAlloc--;
}

/*******************  FUNCTION  *********************/
bool SmallChunkRun::contain ( void* ptr ) const
{
	return (ptr >= data+skipedSize+bitmapEntries/MACRO_ENTRY_BITS && ptr < addrOffset(data,STORAGE_SIZE));
}

/*******************  FUNCTION  *********************/
bool SmallChunkRun::getBitStatus ( SmallSize id )
{
	uint64_t value = *getMacroEntry(id);
	return (value & (1UL << (id & MACRO_ENTRY_MASK))) != 0;
}

/*******************  FUNCTION  *********************/
void SmallChunkRun::setBitStatusOne ( SmallSize id )
{
	uint64_t * value = getMacroEntry(id);
	*value |= (1UL << (id & MACRO_ENTRY_MASK));
}

/*******************  FUNCTION  *********************/
void SmallChunkRun::setBitStatusZero ( SmallSize id )
{
	uint64_t * value = getMacroEntry(id);
	*value &= ~(1UL << (id & MACRO_ENTRY_MASK));
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
bool SmallChunkRun::isFull ( void ) const
{
	allocAssert(this->splitting > 0);
	SmallSize macroEntries = bitmapEntries / MACRO_ENTRY_BITS;
	const MacroEntry * entries = getMacroEntry(0);
	for(SmallSize i = 0 ; i < macroEntries ; i++)
		if (entries[i])
			return false;
	return true;
}

/*******************  FUNCTION  *********************/
void* SmallChunkRun::malloc ( size_t size, size_t align, bool* zeroFilled )
{
	//check size
	allocAssume(size <= splitting,"SmallChunkRun only support allocation smaller than the splitting size.");
	allocCondWarning(size >= splitting / 2,"Caution, you allocate chunk in SmallChunkRun with size less than halfe of the quantum size.");
	
	//search first bit to one (availble free bloc)
	SmallSize macroEntries = bitmapEntries / MACRO_ENTRY_BITS;
	MacroEntry * entries = getMacroEntry(0);
	for(SmallSize i = 0 ; i < macroEntries ; i++)
	{
		//if get one bit to 1, it contain free elements
		if (entries[i])
		{
			//search the first bit to one
			SmallSize id = fastLog2(entries[i]);
			allocAssert(id < MACRO_ENTRY_BITS);
			id += i * MACRO_ENTRY_BITS;
			allocAssert(getBitStatus(id) == true);
			setBitStatusZero(id);
			allocAssert(getBitStatus(id) == false);
			cntAlloc++;
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

/*******************  FUNCTION  *********************/
SmallChunkRun* SmallChunkRun::getFromListHandler ( ListElement* list )
{
	char * ptr = (char*)list;
	ptr -= offsetof(SmallChunkRun,listHandler);
	SmallChunkRun * res = (SmallChunkRun*)ptr;
	allocAssert(&res->listHandler == list);
	return res;
}

/*******************  FUNCTION  *********************/
ListElement* SmallChunkRun::getListHandler ( void )
{
	return &listHandler;
}

};
