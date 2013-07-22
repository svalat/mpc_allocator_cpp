#ifndef MEDIUM_FREE_POOL_H
#define MEDIUM_FREE_POOL_H

/********************  HEADERS  *********************/
#include "Common.h"
#include "MediumChunk.h"
#include "DoubleLinkList.h"

/*********************  TYPES  **********************/
typedef int (*ReverseAnalyticFreeSize)(Size size,const Size * sizeList,int nbLists);
typedef DoubleLinkList<MediumChunk> ChunkFreeList;

/*********************  CONSTS  *********************/
extern const Size cstDefaultFreeSizes[NB_FREE_LIST];
int reverseDefaultFreeSizes(Size size,const Size * sizeList,int nbLists);

/********************  ENUM  ************************/
enum ChunkInsertMode
{
	CHUNK_INSERT_FIFO,
	CHUNK_INSERT_LIFO
};

/*********************  CLASS  **********************/
class MediumFreePool
{
	public:
		MediumFreePool(void);
		MediumFreePool(const Size freeSizes[NB_FREE_LIST],ReverseAnalyticFreeSize analyticRevers = NULL);
		void insert( void* ptr, Size size, ChunkInsertMode mode );
		void insert(MediumChunk * chunk,ChunkInsertMode mode);
		void remove(MediumChunk * chunk);
		MediumChunk * findChunk(Size innerSize);
		MediumChunk * merge(MediumChunk * chunk);
	private:
		void init(const Size freeSizes[NB_FREE_LIST],ReverseAnalyticFreeSize analyticRevers);
		ChunkFreeList * getFreeList(Size innerSize);
		ChunkFreeList * getFreeListByDichotomy(Size innerSize);
		ChunkFreeList * getFreeListByAnalytic(Size innerSize);
		Size getListClass(const ChunkFreeList * list) const;
		void setEmptyStatus(ChunkFreeList * flist,bool filled);
		MediumChunk * findAdaptedChunk( ChunkFreeList* list, Size totalSize );
		ChunkFreeList * getFirstNextNonEmptyList(ChunkFreeList * list);
	private:
		//forbidden copy/=
		MediumFreePool ( const MediumFreePool& other );
		MediumFreePool& operator= ( const MediumFreePool& other );
	private:
		const Size * sizes;
		bool status[NB_FREE_LIST];
		ChunkFreeList lists[NB_FREE_LIST];
		int nbLists;
		ReverseAnalyticFreeSize analyticRevers;
};

#endif // MEDIUM_FREE_POOL_H
