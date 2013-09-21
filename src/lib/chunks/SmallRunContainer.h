#ifndef SMALL_RUN_CONTAINER_H
#define SMALL_RUN_CONTAINER_H

/********************  HEADERS  *********************/
#include <cstdlib>
#include <SmallChunkRun.h>
#include <RegionRegistry.h>
#include <DoubleLinkList.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  TYPES  **********************/
typedef DoubleLinkList<SmallChunkRun> SmallChunkRunList;

/*********************  STRUCT  *********************/
class SmallRunContainer
{
	public:
		static SmallRunContainer * setup(void * ptr,Size size);
		bool isEmpty(void);
		void regEmpty(SmallChunkRun * run);
		SmallChunkRun * getEmptyRun(void);
		ListElement * getListHandler(void);
		static SmallRunContainer * getFromListHandler(ListElement * list);
	private:
		SmallRunContainer(Size size);
		void setupSplitting(void);
	private:
		//forbidden
		SmallRunContainer(const SmallRunContainer & orig);
		~SmallRunContainer(void);
		SmallRunContainer & operator = (const SmallRunContainer & orig);
	private:
		ListElement listHandler;
		SmallChunkRunList emptyRuns;
		Size size;
		uint32_t reservedRuns;
};

};

#endif //SMALL_RUN_CONTAINER_H
