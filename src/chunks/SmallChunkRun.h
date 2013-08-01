#ifndef SMALL_CHUNK_RUN_H
#define SMALL_CHUNK_RUN_H

/********************  HEADERS  *********************/
#include <Common.h>
#include <IAllocator.h>
#include <IDoubleLinkListElement.h>
#include <stdint.h>

/*********************  TYPES  **********************/
typedef uint64_t MacroEntry;
typedef uint16_t SmallSize;

/********************  MACRO  ***********************/
#define SMALL_RUN_SIZE      4096
#define MACRO_ENTRY_BITS    (8*sizeof(MacroEntry))
#define MACRO_ENTRY_SIZE    sizeof(MacroEntry)
#define MACRO_ENTRY_MASK    (MACRO_ENTRY_BITS - 1)
#define STORAGE_SIZE        sizeof(this->data)

/*********************  CLASS  **********************/
class SmallChunkRun : public IDoubleLinkListeElement
{
	public:
		SmallChunkRun(SmallSize skipedSize = 0,SmallSize splitting = 0);
		void setSplitting( SmallSize size );
		bool isEmpty(void) const;
		void * malloc(size_t size,size_t align = BASIC_ALIGN,bool * zeroFilled = NULL);
		void free ( void* ptr );
		size_t getInnerSize ( void* ptr );
		size_t getRequestedSize ( void* ptr );
		size_t getTotalSize ( void* ptr );
		SmallSize getSplitting(void) const;
		void* realloc ( void* ptr, size_t size );
		bool contain(void * ptr) const;
		ListElement * getListHandler(void);
		static SmallChunkRun * getFromListHandler(ListElement * list);
	private:
		void setBitStatusOne(SmallSize id);
		void setBitStatusZero(SmallSize id);
		bool getBitStatus(SmallSize id);
		int getRoundedNbEntries(SmallSize size) const;
		MacroEntry * getMacroEntry( SmallSize id );
	private:
		MacroEntry data[SMALL_RUN_SIZE / sizeof(MacroEntry) - 3];
		ListElement listHandler;
		SmallSize cntAlloc;
		SmallSize skipedSize;
		SmallSize splitting;
		SmallSize bitmapEntries;
};

#endif //SMALL_CHUNK_RUN_H
