#ifndef STDPOOL_H
#define STDPOOL_H

#include "Common.h"
#include "Chunk.h"
#include "DoubleLinkList.h"

extern const Size cstDebugFreeSizes[NB_FREE_LIST];

typedef int (*ReverseAnalyticFreeSize)(Size size,const Size * sizeList,int nbLists);
typedef DoubleLinkList<Chunk> StdPoolList;

class StdPool
{
	public:
		StdPool(const Size freeSizes[NB_FREE_LIST],int nbLists,ReverseAnalyticFreeSize * analytic = NULL);
	private:
		StdPool ( const StdPool& other );
		virtual StdPool& operator= ( const StdPool& other );
	private:
		const Size * sizes;
		bool status[NB_FREE_LIST];
		StdPoolList lists[NB_FREE_LIST];
		int nbLists;
};

#endif // STDPOOL_H
