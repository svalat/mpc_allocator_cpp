/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef SMALL_CHUNK_RUN_H
#define SMALL_CHUNK_RUN_H

/********************  HEADERS  *********************/
#include <Common.h>
#include <IAllocator.h>
#include <IDoubleLinkListElement.h>
#include <stdint.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  TYPES  **********************/
typedef uint64_t MacroEntry;
typedef uint16_t SmallSize;
class SmallRunContainer;

/********************  MACRO  ***********************/
#define SMALL_RUN_SIZE      4096u
#define MACRO_ENTRY_BITS    (8*sizeof(MacroEntry))
#define MACRO_ENTRY_SIZE    sizeof(MacroEntry)
#define MACRO_ENTRY_MASK    (MACRO_ENTRY_BITS - 1)
#define STORAGE_SIZE        sizeof(this->data)

/*********************  CLASS  **********************/
class SmallChunkRun : public IDoubleLinkListeElement
{
	public:
		SmallChunkRun(SmallSize skipedSize = 0,SmallSize splitting = 0,SmallRunContainer * container = NULL);
		void setSplitting( SmallSize size );
		bool isEmpty(void) const;
		bool isFull(void) const;
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
		SmallRunContainer * getContainer(void);
	private:
		void setBitStatusOne(SmallSize id);
		void setBitStatusZero(SmallSize id);
		bool getBitStatus(SmallSize id);
		int getRoundedNbEntries(SmallSize size) const;
		MacroEntry * getMacroEntry( SmallSize id );
		const MacroEntry * getMacroEntry( SmallSize id ) const;
	private:
		MacroEntry data[SMALL_RUN_SIZE / sizeof(MacroEntry) - 4];
		SmallRunContainer * container;
		ListElement listHandler;
		SmallSize cntAlloc;
		SmallSize skipedSize;
		SmallSize splitting;
		SmallSize bitmapEntries;
};

};

#endif //SMALL_CHUNK_RUN_H
