/********************  HEADERS  *********************/
#include "MPSCFQueue.h"
#include "Debug.h"

/*******************  FUNCTION  *********************/
MPSCFQueue::~MPSCFQueue ( void )
{
	allocAssume(isEmpty(),"Can't cleanup a non empty queue.");
}

/*******************  FUNCTION  *********************/
bool MPSCFQueue::isEmpty ( void )
{
	return head.atomicLoad() == NULL;
}

/*******************  FUNCTION  *********************/
void MPSCFQueue::insert ( void* entry, size_t size )
{
	//vars
	MPSCFQueueElement * cur = (MPSCFQueueElement*)entry;
	MPSCFQueueElement * prev;
	
	//errors
	allocAssert(this != NULL);
	allocAssert(entry != NULL);
	allocAssert((Addr) entry % sizeof(MPSCFQueueElement) == 0);
	allocAssert(size >= sizeof(MPSCFQueueElement));

	//this is the new last element, so next is NULL
	cur->next = NULL;
	
	//update tail with swap first
	prev = tail.atomicSwap(cur);
	
	//Then update head if required or update prev->next
	//This operation didn't required atomic ops as long as we are aligned in memory
	if (prev == NULL) {
		//in theory atomic isn't required for this write otherwise we can do atomic write
		head.atomicStore(cur);
	} else {
		prev->next = cur;
	}
}

/*******************  FUNCTION  *********************/
void MPSCFQueue::waitUntilEndId ( MPSCFQueueElement* head, MPSCFQueueElement* expectedTail )
{
	//vars
	volatile struct MPSCFQueueElement * current = head;

	//errors
	allocAssert(current != NULL);
	allocAssert(expectedTail != NULL);

	//loop until we find tail
	while (current != expectedTail)
	{
		if (current->next != NULL)
			current = current->next;
		else
			Atomics::pause();
	}

	//check that we have effectively the last element otherwise it's a bug.
	allocAssert(current->next == NULL);
}

/*******************  FUNCTION  *********************/
MPSCFQueueElement* MPSCFQueue::dequeueAll ( void )
{
	//vars
	struct MPSCFQueueElement * localHead;
	struct MPSCFQueueElement * localTail;

	//errors
	allocAssert(this != NULL);

	// read head and mark it as NULL
	localHead = head.atomicLoad();

	//if has entry, need to clear the current list
	if (localHead != NULL)
	{
		/* Mark head as empty, in theory it's ok without atomic write here.
		At register time, head is write only for first element.
		as we have one, produced work only on tail.
		We will flush tail after this, so it's ok with cache coherence if the two next
		ops are not reorder.*/
		head.atomicStore(NULL);
		//OPA_write_barrier();

		//swap tail to make it NULL
		localTail = tail.atomicSwap(NULL);

		//we have head, so NULL tail is abnormal
		allocAssert(localTail != NULL);

		/* walk on the list until last element and check that it was
		tail, otherwise, another thread is still in registering the tail entry
		but didn't finish to setup ->next, so wait unit next became tail*/
		waitUntilEndId(localHead,localTail);
	}

	/* now we can return */
	return localHead;
}
