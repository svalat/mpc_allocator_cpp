/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef MPSCF_QUEUE_H
#define MPSCF_QUEUE_H

/********************  HEADERS  *********************/
#include "AtomicPtr.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  STRUCT  *********************/
struct MPSCFQueueElement
{
	MPSCFQueueElement * volatile next;
};

/*********************  CLASS  **********************/
class MPSCFQueue
{
	public:
		~MPSCFQueue(void);
		bool isEmpty( void );
		void insert( void* entry, size_t size );
		MPSCFQueueElement * dequeueAll(void);
	private:
		void waitUntilEndId(MPSCFQueueElement * head,MPSCFQueueElement * expectedTail);
	private:
		AtomicPtr<MPSCFQueueElement> head;
		AtomicPtr<MPSCFQueueElement> tail;
};

};

#endif //MPSCF_QUEUE_H
