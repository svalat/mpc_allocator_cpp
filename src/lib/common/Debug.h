/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef DEBUG_H
#define DEBUG_H

/********************  HEADERS  *********************/
#include "Common.h"

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/********************  ENUM  ************************/
enum MessageLevel
{
	MESSAGE_DEBUG        = 0,
	MESSAGE_INFO         = 1,
	MESSAGE_TRACE        = 2,
	MESSAGE_WARNING      = 3,
	MESSAGE_ERROR        = 4,
	MESSAGE_PERROR       = 5,
	MESSAGE_FATAL        = 6,
	MESSAGE_FATAL_PERROR = 7,
	MESSAGE_ASSERT       = 8
};

/*********************  STRUCT  *********************/
class DebugMessage
{
	public:
		DebugMessage(MessageLevel level,const char * file, const char * func,int line);
		void print(const char * format,...);
	private:
		const char * file;
		const char * func;
		int line;
		MessageLevel level;
};

/********************  MACRO  ***********************/
#define MESSAGE_CODE_LOCATION __FILE__, __FUNCTION__, __LINE__

/********************  MACRO  ***********************/
#ifndef NDEBUG
	#define allocAssert(x)      do { if (!(x)) MPCAllocator::DebugMessage(MPCAllocator::MESSAGE_ASSERT , MESSAGE_CODE_LOCATION).print("%s",#x);  } while(0)
	#define allocCondDebug(x,m) do { if (!(x)) MPCAllocator::DebugMessage(MPCAllocator::MESSAGE_DEBUG  , MESSAGE_CODE_LOCATION).print("%s",(m)); } while(0)
	#define allocDebug          MPCAllocator::DebugMessage(MPCAllocator::MESSAGE_DEBUG, MESSAGE_CODE_LOCATION).print
#else
	#define allocAssert( x )    do{} while(0)
	#define allocCondDebug(x,m) do{} while(0)
	#define allocDebug(...)     do{} while(0)
#endif

/********************  MACRO  ***********************/
#define TRACE
#ifdef TRACE
	#define allocTrace          MPCAllocator::DebugMessage(MPCAllocator::MESSAGE_TRACE, MESSAGE_CODE_LOCATION).print
#else
	#define allocTrace(...)     do{} while(0)
#endif

/********************  MACRO  ***********************/
#define allocFatal       MPCAllocator::DebugMessage(MPCAllocator::MESSAGE_FATAL       , MESSAGE_CODE_LOCATION).print
#define allocFatalPerror MPCAllocator::DebugMessage(MPCAllocator::MESSAGE_FATAL_PERROR, MESSAGE_CODE_LOCATION).print
#define allocPerror      MPCAllocator::DebugMessage(MPCAllocator::MESSAGE_PERROR      , MESSAGE_CODE_LOCATION).print
#define allocWarning     MPCAllocator::DebugMessage(MPCAllocator::MESSAGE_WARNING     , MESSAGE_CODE_LOCATION).print

/********************  MACRO  ***********************/
#define allocAssume(x,m)        do { if (!(x)) allocFatal((m));       } while(0)
#define allocAssumePerror(x,m)  do { if (!(x)) allocFatalPerror((m)); } while(0)
#define allocCondWarning(x,m)   do { if (!(x)) allocWarning((m));     } while(0)

/********************  MACRO  ***********************/
#define allocUnused(x)          ((void)x)

};

#endif //DEBUG_H
