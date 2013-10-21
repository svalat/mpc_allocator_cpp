/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <cstdlib>
#include "Debug.h"
#include "TypeToJson.h"
#include "MediumChunk.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*******************  FUNCTION  *********************/
MediumChunk* MediumChunk::setup ( void* ptr, Size totalSize )
{
	//errors
	allocAssert( totalSize >= sizeof(MediumChunk) + BASIC_ALIGN);
	allocAssert( totalSize % BASIC_ALIGN == 0);
	
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
	allocAssert(next > ptr || next == NULL);
	allocAssert(prev < ptr);

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
	allocAssert(prev < ptr);
	allocAssert( totalSize == 0 || totalSize >= sizeof(MediumChunk) + BASIC_ALIGN);
	
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
	allocAssert(res != NULL);
	allocAssert((Addr)ptr % sizeof(Addr) == 0);
	
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
	allocAssert(this != NULL);
	allocAssert(status == CHUNK_FREE || status == CHUNK_ALLOCATED);
	allocAssert(magick == MAGICK_VALUE);
	if (prev != NULL)
		allocAssert(prev->next == this);
	if (next != NULL)
		allocAssert(next->prev == this);
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
	allocAssert(status == CHUNK_FREE || status == CHUNK_ALLOCATED);
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
	allocAssert(this != NULL);
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
	chunk->check();//TODO implementer strict (work without allocAsserts) check and return NULL if not ok
	
	return chunk;
}

/*******************  FUNCTION  *********************/
void MediumChunk::merge ( MediumChunk* last )
{
	MediumChunk * first = this;
	
	//errors
	first->check();
	last->check();
	allocAssert(last >= first);

	if (first == last)
		return;
	
	//compute size
	//Size totalSize = last->next - first;

	if (last->next != NULL)
		last->next->prev = first;
	
	first->next = last->next;
}

/*******************  FUNCTION  *********************/
void typeToJsonInner ( htopml::JsonState& json, std::ostream& stream, const MediumChunk& value )
{
	json.printField("__mem_address__",(void*)&value);
	json.printField("__class_name__","MediumChunk");
	json.printField("status",value.status);
	json.printField("magick",value.magick);
	json.printField("innerSize",value.getInnerSize());
	json.printField("next",(void*)value.getNext());
	json.printField("prev",(void*)value.getPrev());
}

/*******************  FUNCTION  *********************/
void typeToJson ( htopml::JsonState& json, std::ostream& stream, const MediumChunk& value )
{
	const MediumChunk * cur;
	bool first = true;

	json.openStruct();
	typeToJsonInner(json,stream,value);
	
	/////////////////////////////////
// 	json.openField("__mem_objects__");
// 	json.openArray();
// 	bool first = true;
// 	
// 	const MediumChunk * cur = value.getPrev();
// 	while (cur != NULL)
// 	{
// 		if (!first)
// 			stream << ",";
// 		first = false;
// 		json.openStruct();
// 		typeToJsonInner(json,stream,*cur);
// 		json.closeStruct();
// 		stream << ",";
// 		cur = cur->getPrev();
// 	}
// 	
// 	cur = value.getNext();
// 	while (cur != NULL)
// 	{
// 		if (!first)
// 			stream << ",";
// 		first = false;
// 		json.openStruct();
// 		typeToJsonInner(json,stream,*cur);
// 		json.closeStruct();
// 		cur = cur->getNext();
// 	}
// 	
// 	json.closeArray();
// 	json.closeField("__mem_objects__");
	
	/////////////////////////////////
	json.openField("__mem_links__");
	json.openArray();
	cur = &value;
	first = true;
	while (cur != NULL)
	{
		if (!first)
			stream << ",";
		first = false;
		json.openStruct();
		json.printField("from",(void*)cur);
		json.printField("to",(void*)cur->getPrev());
		json.closeStruct();
		stream << ",";
		json.openStruct();
		json.printField("from",(void*)cur);
		json.printField("to",(void*)cur->getNext());
		json.closeStruct();
		cur = cur->getPrev();
	}
	
	cur = value.getNext();
	while (cur != NULL)
	{
		if (!first)
			stream << ",";
		first = false;
		json.openStruct();
		json.printField("from",(void*)cur);
		json.printField("to",(void*)cur->getPrev());
		json.closeStruct();
		stream << ",";
		json.openStruct();
		json.printField("from",(void*)cur);
		json.printField("to",(void*)cur->getNext());
		json.closeStruct();
		cur = cur->getNext();
	}
	
	json.closeArray();
	json.closeField("__mem_links__");
	
	json.closeStruct();
}

/*******************  FUNCTION  *********************/
ListElement* MediumChunk::getListHandler ( void )
{
	allocAssert(this != NULL);
	allocAssert(getStatus() == CHUNK_FREE);
	return (ListElement*) (this+1);
}

/*******************  FUNCTION  *********************/
MediumChunk * MediumChunk::getFromListHandler ( ListElement* list )
{
	MediumChunk * res = NULL;
	if (list != NULL)
		res = ((MediumChunk*)list) - 1;
	res->check();
	return res;
}

/*******************  FUNCTION  *********************/
bool MediumChunk::contain ( void* ptr )
{
	return (ptr >= getPtr() && ptr < addrOffset(getPtr(),getInnerSize()));
}

};
