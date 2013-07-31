#ifndef CHUNK_H
#define CHUNK_H

/********************  HEADERS  *********************/
#include "Common.h"
#include <IDoubleLinkListElement.h>
#include <JsonState.h>

/********************  ENUM  ************************/
enum ChunkStatus
{
	CHUNK_FREE,
	CHUNK_ALLOCATED
};

/*********************  STRUCT  *********************/
struct ListElement;

/*********************  CLASS  **********************/
class MediumChunk : public IDoubleLinkListeElement
{
	public:
		static MediumChunk * setup(void * ptr,Size totalSize);//checked
		static MediumChunk * setup(void * ptr,MediumChunk * prev,Size totalSize);//checked
		static MediumChunk * setup(void * ptr,MediumChunk * prev,MediumChunk * next);
		static MediumChunk * getChunk(void * ptr);
		static MediumChunk * getChunkSafe(void * ptr);
		Size getTotalSize(void) const;//checked
		Size getInnerSize(void) const;//checked
		void check(void) const;//checked
		MediumChunk * getNext(void) const;//checked
		MediumChunk * getPrev(void) const;//checked
		ChunkStatus getStatus(void) const;//checked
		void setStatus(ChunkStatus status);//checked
		MediumChunk * split(Size innerSize);//checked
		bool isSingle(void) const;//checked
		void * getPtr(void);
		void merge( MediumChunk* last );
		ListElement * getListHandler(void);
		static MediumChunk * getFromListHandler(ListElement * list);
	private:
		static MediumChunk * setup(void * ptr);
	private:
		//forbidden
		MediumChunk(void);
		MediumChunk ( const MediumChunk& other );
		MediumChunk& operator= ( const MediumChunk& other );
		~MediumChunk(void);
	private:
		MediumChunk * next;
		MediumChunk * prev;
		uint32_t status;
		uint32_t magick;
	public:
		friend void typeToJson(htopml::JsonState& json, std::ostream& stream, const MediumChunk & value);
		friend void typeToJsonInner ( htopml::JsonState& json, std::ostream& stream, const MediumChunk& value );
};

#endif // CHUNK_H
