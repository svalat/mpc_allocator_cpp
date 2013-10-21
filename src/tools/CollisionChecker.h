/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef COLLISION_CHECKER_H
#define COLLISION_CHECKER_H

/********************  HEADERS  *********************/
#include <cstdlib>
#include <stdint.h>
#include <set>

/*********************  STRUCT  *********************/
class CheckerMemorySegment
{
	public:
		CheckerMemorySegment(uint64_t base,uint64_t size);
		CheckerMemorySegment(void * base,size_t size);
		CheckerMemorySegment(const CheckerMemorySegment & orig);
		uint64_t getBase(void) const;
		uint64_t getSize(void) const;
		friend bool operator <(const CheckerMemorySegment & v1,const CheckerMemorySegment & v2);
		friend bool operator >(const CheckerMemorySegment & v1,const CheckerMemorySegment & v2);
		friend bool operator == (const CheckerMemorySegment & v1,const CheckerMemorySegment & v2);
	private:
		uint64_t base;
		uint64_t size;
};

/*********************  TYPES  **********************/
typedef std::set<CheckerMemorySegment> CheckerMemorySegmentDeque;

/*********************  CLASS  **********************/
class CollisionChecker
{
	public:
		void registerSegment(void * ptr,size_t size);
		void unregisterSegment(void * ptr,size_t size);
	private:
		CheckerMemorySegmentDeque segments;
};

#endif //COLLISION_CHECKER_H
