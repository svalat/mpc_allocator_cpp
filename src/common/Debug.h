#ifndef DEBUG_H
#define DEBUG_H

/********************  HEADERS  *********************/
#include "Common.h"

/********************  ENUM  ************************/
enum MessageLevel
{
	MESSAGE_DEBUG        = 0,
	MESSAGE_INFO         = 1,
	MESSAGE_WARNING      = 2,
	MESSAGE_ERROR        = 3,
	MESSAGE_PERROR       = 4,
	MESSAGE_FATAL        = 5,
	MESSAGE_FATAL_PERROR = 6,
	MESSAGE_ASSERT       = 7
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
	#define allocAssert(x)      do { if (!(x)) DebugMessage(MESSAGE_ASSERT , MESSAGE_CODE_LOCATION).print(#x);  } while(0)
	#define allocCondDebug(x,m) do { if (!(x)) DebugMessage(MESSAGE_DEBUG  , MESSAGE_CODE_LOCATION).print((m)); } while(0)
	#define allocDebug          DebugMessage(MESSAGE_DEBUG, MESSAGE_CODE_LOCATION).print
#else
	#define allocAssert( x )    do{} while(0)
	#define allocCondDebug(x,m) do{} while(0)
	#define allocDebug(...)     do{} while(0)
#endif

/********************  MACRO  ***********************/
#define allocFatal       DebugMessage(MESSAGE_FATAL       , MESSAGE_CODE_LOCATION).print
#define allocFatalPerror DebugMessage(MESSAGE_FATAL_PERROR, MESSAGE_CODE_LOCATION).print
#define allocPerror      DebugMessage(MESSAGE_PERROR      , MESSAGE_CODE_LOCATION).print

/********************  MACRO  ***********************/
#define allocAssume(x,m)        do { if (!(x)) allocFatal((m));       } while(0)
#define allocAssumePerror(x,m)  do { if (!(x)) allocFatalPerror((m)); } while(0)

#endif //DEBUG_H
