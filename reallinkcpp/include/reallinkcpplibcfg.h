#ifndef REALLINKCPP_LIBCFG_H
#define REALLINKCPP_LIBCFG_H

#include <windows.h>

namespace reallinkcpp
{

#define REALLINKCPP_LIB_VERSION	"1.0.0"


#if defined REALLINKCPP_LIB_STATIC
	#define REALLINKCPP_LIB_DLL
#else
	#if defined REALLINKCPP_LIB_EXPORTS
		#define REALLINKCPP_LIB_DLL __declspec(dllexport)
	#else
		#define REALLINKCPP_LIB_DLL __declspec(dllimport)
	#endif
#endif


}//namespace

#endif
