#pragma once
#include <memory>

#ifdef KE_PLATFORM_WINDOWS 
#if KE_DYNAMIC_LINK
	#ifdef KE_BUILD_DLL
		#define KINGO_API __declspec(dllexport)
	#else
		#define KINGO_API __declspec(dllimport)
	#endif
#else
	#define KINGO_API
#endif
#else
	#error Kingo only supports Windows!
#endif

#ifdef KE_DEBUG
	#define KE_ENABLE_ASSERTS
#endif

#ifdef KE_ENABLE_ASSERTS
	#define KE_ASSERT(x, ...) { if(!(x)) { KE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define KE_CORE_ASSERT(x, ...) { if(!(x)) { KE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define KE_ASSERT(x, ...)
	#define KE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define KE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Kingo {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}