/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef COMMON_H
#define COMMON_H

/********************  HEADERS  *********************/
#include <cstdlib>
#include <cassert>
#include <stdint.h>

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  TYPES  **********************/
typedef uint64_t Addr;
typedef uint64_t Size;
typedef int64_t SSize;

/********************  MACRO  ***********************/
#define BASIC_ALIGN sizeof(Addr)
#define MAGICK_VALUE 42
#define NB_FREE_LIST 50
#define UNSUPPORTED 0u
#define ALLOC_MIN_SIZE (2*BASIC_ALIGN)
//TODO setup value
#define REALLOC_THREASHOLD 64
#define MACRO_BLOC_SIZE (2*1024*1024UL)
#define MEDIUM_MIN_INNER_SIZE 16
#define ADDR_NULL 0
#define PAGE_SIZE 4096
#define ALLOC_DO_WARNING true
#define HUGE_ALLOC_THREASHOLD (MACRO_BLOC_SIZE/2)

/********************  MACRO  ***********************/
//TODO check to by use in coordination with -fvisibility=hidden for seom optim
// #define DLL_PUBLIC __attribute__ ((visibility ("default")))
// #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))

/********************  MACRO  ***********************/
#define allocOffsetOf(className,memberName) ((Addr)&((className *)32)->memberName - (Addr)(32))

/*********************  TYPES  **********************/
typedef void(*AbortHandler)(void);

/*******************  FUNCTION  *********************/
void abort(void);
AbortHandler setAbortHandler( MPCAllocator::AbortHandler handler );

/*******************  FUNCTION  *********************/
static inline Size ceilToPowOf2(Size size,Size align)
{
	size &= ~(align-1);
	return size;
}

/*******************  FUNCTION  *********************/
static inline Size upToPowOf2(Size size,Size align)
{
	if (size & (align-1))
		size = (size & ~(align-1))+align;
	return size;
}

/*******************  FUNCTION  *********************/
template <class T>
T * addrOffset(T * value,SSize offset)
{
	assert(value != NULL);
	return (T*)((Addr)value + offset);
}

/*******************  FUNCTION  *********************/
static inline long addrDelta(void * p1,void * p2)
{
	return (char*)p1 - (char*)p2;
}

/*******************  FUNCTION  *********************/
template <class T>
T max(T a,T b)
{
	if (a>b)
		return a;
	else
		return b;
}

/*******************  FUNCTION  *********************/
template <class T>
T min(T a,T b)
{
	if (a<b)
		return a;
	else
		return b;
}

/*******************  FUNCTION  *********************/
static inline int fastLog2(Size value)
{
	//vars
	Size res = 0;

	#if defined(__GNUC__) && defined(__x86_64__)
		if (value != 0)
			asm volatile ("bsr %1, %0":"=r" (res):"r"(value));
	#else
		/** @TODO find equivalent for others compiler. Maybe arch x86 is also OK as for x86_64, but need to check. **/
		#ifndef _MSC_VER
			#warning "ASM bsr was tested only on gcc x64_64, fallback on default slower C implementation."
		#endif
		while (value > 1) {value = value >> 1 ; res++;};
	#endif

	return (int)res;
}

/*******************  FUNCTION  *********************/
static inline bool checkNoOverlap(const void * ptr1, Size size1,const void * ptr2, Size size2)
{
	return ((Addr)ptr1 + size1 <= (Addr)ptr2 || (Addr)ptr2 + size2 <= (Addr)ptr1);
}

};

#endif //COMMON_H
