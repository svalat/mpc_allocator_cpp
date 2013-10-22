/*****************************************************
*            PROJECT  : MPC_Allocator_CPP            *
*            VERSION  : 0.0.0                        *
*            DATE     : 07/2013                      *
*            AUTHOR   : Valat Sébastien              *
*            LICENSE  : CeCILL-C                     *
*****************************************************/

#ifndef SYMBOL_VSISIBILITY_H
#define SYMBOL_VSISIBILITY_H

/********************  MACRO  ***********************/
//Came from http://gcc.gnu.org/wiki/Visibility
//Others compiler provide similar tweaks. Here it may be OK for GCC, clang and VCC
#if ENABLE_VISIBILITY_TWEAK
	#if defined _WIN32 || defined __CYGWIN__
		#ifdef BUILDING_DLL
			#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__ ((dllexport))
			#else
			#define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
			#endif
		#else
			#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__ ((dllimport))
			#else
			#define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
			#endif
		#endif
		#define DLL_LOCAL
	#else
		#if __GNUC__ >= 4
			#define DLL_PUBLIC __attribute__ ((visibility ("default")))
			#define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
		#else
			#define DLL_PUBLIC
			#define DLL_LOCAL
		#endif
	#endif
#else
	#define DLL_PUBLIC
	#define DLL_LOCAL
#endif

#endif //SYMBOL_VSISIBILITY_H
