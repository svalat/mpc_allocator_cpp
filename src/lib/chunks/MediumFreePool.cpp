/********************  HEADERS  *********************/
#include "MediumFreePool.h"
#include <TypeToJson.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

//////////////////////////////////////////////////////////////////////////////////TODO
const Size cstDefaultFreeSizes[NB_FREE_LIST] = {16, 24,
        32,    64,   96,  128,  160,   192,   224,   256,    288,    320,
        352,  384,  416,  448,  480,   512,   544,   576,    608,    640,
        672,  704,  736,  768,  800,   832,   864,   896,    928,    960,
        992, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144,
        524288, 1048576, 2*1024*1024, -1UL, -1UL, -1UL, -1UL, -1UL
};

int reverseDefaultFreeSizes(Size size,const Size * sizeList,int nbLists)
{
	//errors
	allocAssert(sizeList == cstDefaultFreeSizes);
	allocAssert(64 >> 5 == 2);
	allocAssert(sizeList[45] == -1UL);
	allocAssert(size >= 16);

	if (size < 32)
		return (int)(size/8) - 2;
	else if (size <= 1024)
		//divide by 32 and fix first element ID as we start to indexes by 0
		// +2 for thre startpoint 16/24
		return (int)((size >> 5) - 1) + 2;
	else if (size > MACRO_BLOC_SIZE)
		// +2 for thre startpoint 16/24
		return 43 + 2;
	else
		//1024/32 :  starting offset of the exp zone
		// >> 10: ( - log2(1024)) remote the start of the exp
		// +2 for thre startpoint 16/24
	return 1024/32 + fastLog2(size >> 10) - 1 + 2;
}
//////////////////////////////////////////////////////////////////////////////////TODO

/*******************  FUNCTION  *********************/
MediumFreePool::MediumFreePool ( void )
{
	init(cstDefaultFreeSizes,reverseDefaultFreeSizes);
}

/*******************  FUNCTION  *********************/
MediumFreePool::MediumFreePool ( const Size freeSizes[48], ReverseAnalyticFreeSize analyticRevers )
{
	init(freeSizes,analyticRevers);
}

/*******************  FUNCTION  *********************/
void MediumFreePool::init ( const Size freeSizes[48], ReverseAnalyticFreeSize analyticRevers )
{
	allocAssert(freeSizes != NULL);
	allocAssert(this != NULL);
	allocAssert(freeSizes[NB_FREE_LIST-1]==-1UL);
	
	//setup sizes
	this->sizes = freeSizes;
	this->nbLists = 0;
	this->analyticRevers = analyticRevers;
	
	//count sizes
	for (int i = 0 ; i < NB_FREE_LIST ; i++)
	{
		status[i] = false;
		if (sizes[i] != -1UL)
			nbLists=i+1;
	}
	
	 //keep the list -1
	if (nbLists < NB_FREE_LIST)
	{
		if (nbLists%2 == 0)
				nbLists+=2;
		else
				nbLists++;
		//TODO
		allocAssert(nbLists < NB_FREE_LIST);
		//assume_m(pool->nb_free_lists < NB_FREE_LIST,"Error while calculating number of free lists.");
	} else {
		//TODO
		allocAssert(false);
		//warning("Caution, the last free list size must be -1, you didn't follow this requirement, this may leed to errors.");
	}
}

/*******************  FUNCTION  *********************/
ChunkFreeList* MediumFreePool::getFreeList ( Size innerSize )
{
	//errors
	allocAssert(this != NULL);
	allocAssert(sizes != NULL);
	allocAssert( innerSize > 0);
	
	if (analyticRevers == NULL)
		return getFreeListByDichotomy( innerSize );
	else
		return getFreeListByAnalytic( innerSize );
}

/*******************  FUNCTION  *********************/
ChunkFreeList* MediumFreePool::getFreeListByDichotomy ( Size innerSize )
{
	//local vars
	Size seg_size = nbLists;
	Size i = seg_size >> 1;
	const Size * ptr = sizes;
	
	//errors
	allocAssert(ptr != NULL);
	allocAssert(this != NULL);
	allocAssert( innerSize > 0);
	allocAssert(4 >> 1 == 2);//required property to quickly divide by 2
	allocAssert(innerSize >= ptr[0]);
	
	if (ptr[0] >= innerSize )
	{
		i = 0;
	} else {
		//use dicotomic search, it's faster as we know the list sizes are sorted.
		while((ptr[i-1] >= innerSize || ptr[i] < innerSize ))
		{
			if (ptr[i] < innerSize )
			{
				seg_size -= i;
				ptr += i;
			} else {
				seg_size = i;
			}
			i = seg_size >> 1;//divide by 2
		}
	}
	allocAssert(i >= 0 && ptr+i >= this->sizes);
	
	return lists+(ptr-sizes+i);
}

/*******************  FUNCTION  *********************/
ChunkFreeList* MediumFreePool::getFreeListByAnalytic ( Size innerSize )
{
	//vars
	const Size * sizeList = this->sizes;
	ReverseAnalyticFreeSize analyticRevers = this->analyticRevers;
	int pos;
	
	//errors
	allocAssert( innerSize > 0);
	allocAssert(this != NULL);
	allocAssert(sizeList != NULL);
	allocAssert(analyticRevers != NULL);

	//get position by reverse analytic computation.
	pos = analyticRevers( innerSize, sizeList,nbLists);

	//check size of current cell, if too small, take the next one
	if (sizeList[pos] < innerSize )
		pos++;

	//check
	allocAssert(pos >= 0 && pos <= nbLists);
	allocAssert(lists + pos == getFreeListByDichotomy( innerSize ));

	//return position
	return lists + pos;
}

/*******************  FUNCTION  *********************/
Size MediumFreePool::getListClass ( const ChunkFreeList* list ) const
{
	int id = (int)(list - lists);
	#ifndef SCTK_ALLOC_FAST_BUT_LESS_SAFE
	allocAssert(id >= 0 && id < NB_FREE_LIST);
	//assume_m(id >= 0 && id < NB_FREE_LIST,"The given list didn't be a member of the given thread pool.");
	#endif
	return sizes[id];
}

/*******************  FUNCTION  *********************/
void MediumFreePool::insert ( MediumChunk* chunk,ChunkInsertMode mode )
{
	//get size
	chunk->check();
	Size innerSize = chunk->getInnerSize();

	//errors
	allocAssert(this != NULL);
	allocAssert(chunk->getInnerSize() >= sizeof(ChunkFreeList));
	allocAssert(chunk->getTotalSize() > 0);
	allocAssert(chunk->getStatus() == CHUNK_ALLOCATED);
	
	//get the free list
	ChunkFreeList * flist = getFreeList(innerSize);
	allocAssert(flist != NULL);
	
	Size listClass = getListClass(flist);
	if (flist != lists && listClass != -1UL && listClass != innerSize)
		flist--;
	
	//mark free
	chunk->setStatus(CHUNK_FREE);
	
	//insert
	switch(mode)
	{
		case CHUNK_INSERT_FIFO:
			flist->putFirst(chunk);
			break;
		case CHUNK_INSERT_LIFO:
			flist->putLast(chunk);
			break;
		default:
			//TODO
			allocAssert(false);
			//assume_m(false,"Unknown insert mode in free list.");
			break;
	}
	
	//mark non empty
	setEmptyStatus(flist,true);
}

/*******************  FUNCTION  *********************/
void MediumFreePool::insert ( void* ptr, Size size,ChunkInsertMode mode )
{
	MediumChunk * chunk = MediumChunk::setup(ptr,size);
	insert(chunk,mode);
}

/*******************  FUNCTION  *********************/
void MediumFreePool::remove ( MediumChunk* chunk )
{
	//errors
	allocAssert(chunk != NULL);
	allocAssert(this != NULL);
	chunk->check();
	allocAssert(chunk->getStatus() == CHUNK_FREE);
	
	ChunkFreeList * list = ChunkFreeList::remove(chunk);
	if (list != NULL)
		setEmptyStatus(list,false);
	
	chunk->setStatus(CHUNK_ALLOCATED);
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumFreePool::findAdaptedChunk ( ChunkFreeList * list, Size innerSize )
{
	//errors
	allocAssert( innerSize >= 0);
	allocAssert(this != NULL);
	allocAssert( list != NULL);
	allocAssert( list >= lists && list < lists + nbLists);

	//first in the list fo oldest one -> FIFO
	ChunkFreeList::Iterator it = list->begin();
	while (it != list->end() && it->getInnerSize() < innerSize )
		++it;
	
	if (it == list->end())
		return NULL;
	else
		return &(*it);
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumFreePool::findChunk ( Size innerSize )
{
	//vars
	MediumChunk * res = NULL;

	//errors
	allocAssert(innerSize > 0);
	allocAssert(this != NULL);
	
	//get the minimum valid size
	ChunkFreeList * list = getFreeList(innerSize);
	ChunkFreeList * startPoint = list;
	
	//if empty list, go to next if available
	//otherwite, take the first of the list (oldest one -> FIFO)
	list = getFirstNextNonEmptyList(list);
	if (list != NULL)
		res = findAdaptedChunk(list,innerSize);
	
	//if not found, try our chance in the previous list (we may find some sufficient bloc, but may
	//require more steps of searching as their may be some smaller blocs in this one on the contrary
	//of our starting point which guaranty to get sufficient size
	if (res == NULL && startPoint != lists)
	{
		list = startPoint - 1;
		res = findAdaptedChunk(list,innerSize);
	}
	
	//if find, remove from list
	if (res != NULL)
		remove(res);
	
	return res;
}

/*******************  FUNCTION  *********************/
ChunkFreeList* MediumFreePool::getFirstNextNonEmptyList ( ChunkFreeList* list )
{
	//errors
	allocAssert(this != NULL);
	allocAssert(list != NULL);
	allocAssert(nbLists <= NB_FREE_LIST);
	allocAssert(list >= lists && list < lists + nbLists);
	
	//get free list id
	int id = (list - lists);
	allocAssert(id < nbLists);
	
	for (int i = id ; i < nbLists ; i++)
	{
		if (status[i])
			return lists+i;
	}

	//don't foudna nything non empty
	return NULL;
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumFreePool::merge ( MediumChunk* chunk )
{
	//vars
	MediumChunk * first = chunk;
	MediumChunk * last = chunk;
	MediumChunk * cur;

	//error
	allocAssert(this != NULL);
	allocAssert(chunk != NULL);
	allocAssert(chunk->getStatus() == CHUNK_ALLOCATED);
	//assume_m(chunk->getStatus() == CHUNK_ALLOCATED,"The central chunk must be allocated to be merged.");
	
	//search for the first free chunk before the central one.
	cur = chunk->getPrev();
	while(cur != NULL && cur->getStatus() == CHUNK_FREE)
	{
		first = cur;
		//can remove current one from free list to be merged at the end of the function
		remove(cur);
		//move to next one
		cur = cur->getPrev();
	}

	//search the last mergeable after the central one
	cur = chunk->getNext();
	while(cur != NULL && cur->getStatus() == CHUNK_FREE)
	{
		last = cur;
		//can remove current one from free list to be merged at the end of the function
		remove(cur);
		//move to next one
		cur = cur->getNext();
	}
	
	//calc final bloc size
	first->merge(last);
	return first;
}

/*******************  FUNCTION  *********************/
void MediumFreePool::setEmptyStatus ( ChunkFreeList* flist, bool filled )
{
	int id = (int)(flist - lists);
	allocAssert(id >= 0 && id < NB_FREE_LIST);
	
	status[id] = filled;
}

/*******************  FUNCTION  *********************/
MediumChunk* MediumFreePool::tryMergeForSize ( MediumChunk* chunk, Size findInnerSize )
{
	//errors
	allocAssert(chunk != NULL);
	allocAssert( findInnerSize > 0);
	allocAssert( findInnerSize > chunk->getInnerSize());
	
	//start to search
	MediumChunk * cur = chunk->getNext();
	MediumChunk * last = chunk;
	Size size = chunk->getInnerSize();
	
	//loop until enought
	while(cur != NULL && cur->getStatus() == CHUNK_FREE && size < findInnerSize)
	{
		size += cur->getTotalSize();
		last = cur;
		//move to next one
		cur = cur->getNext();
	}
	
	//if not enought, return NULL
	if (size < findInnerSize)
		return NULL;
	
	//free all from chunk to last
	MediumChunk * lastNext = cur;
	cur = chunk->getNext();
	//loop until enought
	while(cur != NULL && cur->getStatus() == CHUNK_FREE && cur != lastNext)
	{
		remove(cur);
		//move to next one
		cur = cur->getNext();
	}
	
	//final merge
	chunk->merge(last);
	return chunk;
}

/*******************  FUNCTION  *********************/
void typeToJson ( htopml::JsonState& json, std::ostream& stream, const MediumFreePool& value )
{
	json.openStruct();
	json.printField("nbList",value.nbLists);
	json.printField("analyticRevers",(void*)value.analyticRevers);
	json.printField("__class_name__","MediumFreePool");
	json.printField("__mem_address__",(void*)&value);
	
	json.openField("lists");
	json.openArray();
	
	bool first = true;
	for ( int i = 0 ; i < value.nbLists ; i++)
	{
		if (value.lists[i].isEmpty() == false)
		{
			if (!first)
				stream << ",";
			first = false;
			json.openStruct();
			json.printField("__class_name__","MediumFreePoolVList");
			json.printField("id",i);
			json.printField("size",value.sizes[i]);
			json.printField("status",value.status[i]);
			json.printField("list",value.lists[i]);
			json.closeStruct();
		}
	}
	
	json.closeArray();
	json.closeField("lists");
	
	json.closeStruct();
}

/*******************  FUNCTION  *********************/
void MediumFreePool::hardChecking ( void )
{
	for (int i = 0 ; i < NB_FREE_LIST ; i++)
		lists[i].hardChecking();
}

};
