#pragma once

#ifdef KE_PLATFORM_WINDOWS 
	#ifdef KE_BUILD_DLL
		#define KINGO_API __declspec(dllexport)
	#else
		#define KINGO_API __declspec(dllimport)
	#endif
#else
	#define KINGO_API
	#error Kingo only supports Windows!
#endif

#define BIT(x) (1 << x)