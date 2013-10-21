/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

/********************  HEADERS  *********************/
#include <cassert>
#include <cstring>
#include "BugReproducer.h"
#include "EventDispatchReplay.h"
#include "EventDispatchPrint.h"

/*******************  FUNCTION  *********************/
void BugReproducer::run ( TraceReader& reader )
{
	//save start position
	size_t startPos = reader.getCurrentId();
	
	//search bug position
	size_t bugPos = searchBugPosition(reader);
	
	//use dichotomic search to find the best starting point.
	size_t minimalStartPos = searchStartingPoint(reader,startPos,bugPos);
	
	//prepare mask to remove non impacting actions
	size_t cnt = (bugPos - minimalStartPos) + 1;
	bool * mask = new bool[cnt];
	for (size_t i = 0 ; i < cnt ; i++)
		mask[i] = true;
	
	//prune useless actions to keep only the important one
	pruneActions(reader,minimalStartPos,bugPos,mask);
	
	//print the buggy sequence
	printReproducer(reader,minimalStartPos,bugPos,mask);
	
	//celar mask
	delete [] mask;
}

/*******************  FUNCTION  *********************/
size_t BugReproducer::searchBugPosition ( TraceReader& reader )
{
	EventDispatchReplay replay;
	replay.resetTLS();
	size_t res = 0;

	try {
		replay.run(reader);
		allocFatal("Fail to find the bug, try to use 'throw 42' at bug position if it's a warning.");
	} catch (int a) {
		res = reader.getCurrentId()-1;
		allocDebug("Find bug ID : %lu\n",res);
	}

	return res;
}

/*******************  FUNCTION  *********************/
size_t BugReproducer::searchStartingPoint ( TraceReader& reader, size_t startPos, size_t bugPos )
{
	size_t upperLimit = startPos;
	size_t lowerLimit = bugPos;

	//loop until choice reduced to 0
	while (upperLimit-lowerLimit != 0)
	{
		//calc middle position
		size_t cur = (upperLimit + lowerLimit) / 2;
		if (cur == upperLimit)
		{
			cur = upperLimit;
			lowerLimit = upperLimit;
		}
		allocDebug("Try start position %lu, dichotomic interval is [%lu,%lu], original is [%lu,%lu]",cur,upperLimit,lowerLimit,startPos,bugPos);
		
		//try to play the lower part of the segment
		bool buggy = isBuggySequence(reader,cur,bugPos);
		allocDebug("Status = %d\n",buggy);
		
		//if buggy, keep lower interval, otherwise, upper one
		if (buggy)
			upperLimit = cur;
		else
			lowerLimit = cur;
	}
	
	//debug
	assert(isBuggySequence(reader,upperLimit,bugPos));
	
	return upperLimit;
}

/*******************  FUNCTION  *********************/
bool BugReproducer::isBuggySequence ( TraceReader& reader, size_t startPos, size_t bugPos, const bool * mask )
{
	MPCAllocator::TraceEntryComplete entry;
	EventDispatchReplay replay(false);
	replay.resetTLS();

	bool status;

	try {
		reader.setPosition(startPos);
		for (size_t i = startPos ; i <= bugPos ; i++)
		{
			bool res = reader.readNext(entry);
			assert(res == true);
			allocUnused(res);
			if (mask == NULL || mask[i - startPos] == true)
				replay.run(entry);
		}
		status = false;
	} catch (int a) {
		status = true;
	}
	
	return status;
}

/*******************  FUNCTION  *********************/
void BugReproducer::printReproducer ( TraceReader& reader, size_t startPos, size_t bugPos, const bool * mask )
{
	printf("================================= REPRODUCER ==============================\n");
	printf("//sequence is from %lu to %lu\n",startPos,bugPos);

	reader.reopen();// Need to find why it bug without this reset
	reader.setPosition(startPos);
	EventDispatchPrint printer;
	EventDispatchReplay replay(false);
	replay.resetTLS();
	MPCAllocator::TraceEntryComplete entry;
	
	for (size_t i = startPos ; i <= bugPos ; i++)
	{
		bool res = reader.readNext(entry);
		assert(res == true); allocUnused(res);
		if (mask == NULL || mask[i - startPos] == true)
		{
			//allocDebug("Print action ID = %lu, bug is %lu, firstPos is %lu",i,bugPos, startPos);
			printer.run(entry);
			replay.run(entry);
		}
	}
}

/*******************  FUNCTION  *********************/
void BugReproducer::pruneActions ( TraceReader reader, size_t startPos, size_t bugPos, bool* mask )
{
	//errors
	assert(mask != NULL);
	int cnt = 0;
	
	for( size_t i = startPos ; i < bugPos ; i++)
	{
		allocDebug("Pruning step %lu / %lu => current cnt  = %lu",i - startPos,bugPos - startPos,cnt);
		mask[i-startPos] = false;
		mask[i-startPos] = (isBuggySequence(reader,startPos,bugPos,mask) == false);
		if (mask[i-startPos])
			cnt++;
	}
}
