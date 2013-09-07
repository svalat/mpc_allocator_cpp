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
	#define allocAssert(x)      do { if (!(x)) DebugMessage(MESSAGE_ASSERT , MESSAGE_CODE_LOCATION).print("%s",#x);  } while(0)
	#define allocCondDebug(x,m) do { if (!(x)) DebugMessage(MESSAGE_DEBUG  , MESSAGE_CODE_LOCATION).print("%s",(m)); } while(0)
	#define allocDebug          DebugMessage(MESSAGE_DEBUG, MESSAGE_CODE_LOCATION).print
#else
	#define allocAssert( x )    do{} while(0)
	#define allocCondDebug(x,m) do{} while(0)
	#define allocDebug(...)     do{} while(0)
#endif

/********************  MACRO  ***********************/
#define TRACE
#ifdef TRACE
	#define allocTrace          DebugMessage(MESSAGE_TRACE, MESSAGE_CODE_LOCATION).print
#else
	#define allocTrace(...)     do{} while(0)
#endif

/********************  MACRO  ***********************/
#define allocFatal       DebugMessage(MESSAGE_FATAL       , MESSAGE_CODE_LOCATION).print
#define allocFatalPerror DebugMessage(MESSAGE_FATAL_PERROR, MESSAGE_CODE_LOCATION).print
#define allocPerror      DebugMessage(MESSAGE_PERROR      , MESSAGE_CODE_LOCATION).print
#define allocWarning     DebugMessage(MESSAGE_WARNING     , MESSAGE_CODE_LOCATION).print

/********************  MACRO  ***********************/
#define allocAssume(x,m)        do { if (!(x)) allocFatal((m));       } while(0)
#define allocAssumePerror(x,m)  do { if (!(x)) allocFatalPerror((m)); } while(0)
#define allocCondWarning(x,m)   do { if (!(x)) allocWarning((m));     } while(0)

};

#endif //DEBUG_H
