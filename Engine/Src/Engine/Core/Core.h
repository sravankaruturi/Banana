#pragma once
#include <cstdint>

namespace ee
{

	void InitializeCore();
	void ShutdownCore();

}

#ifdef EE_DEBUG
#define EE_ENABLE_ASSERTS
#endif

#ifdef EE_PLATFORM_WINDOWS

#ifdef EE_ENABLE_ASSERTS

#define EE_ASSERT(x, ...) { if(!(x)) { EE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define EE_CORE_ASSERT(x, ...) { if(!(x)) { EE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#endif // EE_ENABLE_ASSERTS


#endif

#define BIT(x) (1 << x)

using euint = uint64_t;
using eint = int64_t;