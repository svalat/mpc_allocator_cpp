/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef BUG_REPRODUCER_H
#define BUG_REPRODUCER_H

/********************  HEADERS  *********************/
#include "TraceReader.h"

/*********************  CLASS  **********************/
class BugReproducer
{
	public:
		void run(TraceReader & reader);
	private:
		size_t searchBugPosition(TraceReader & reader);
		size_t searchStartingPoint(TraceReader & reader,size_t startPos,size_t bugPos);
		bool isBuggySequence( TraceReader& reader, size_t startPos, size_t bugPos, const bool* mask = NULL );
		void printReproducer( TraceReader& reader, size_t startPos, size_t bugPos, const bool* mask );
		void pruneActions ( TraceReader reader, size_t startPos, size_t bugPos, bool* mask );
};

#endif //BUG_REPRODUCER_H
