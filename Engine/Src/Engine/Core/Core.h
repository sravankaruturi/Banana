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

#define EE_ASSERT(x, ...) { if(!(x)) { EE_ERROR("Assertion Failed: {0} at {1} : {2}", __VA_ARGS__, __FILE__, __LINE__); __debugbreak(); } }
#define EE_CORE_ASSERT(x, ...) { if(!(x)) { EE_CORE_ERROR("Assertion Failed: {0} at {1} : {2}", __VA_ARGS__, __FILE__, __LINE__); __debugbreak(); } }

#endif // EE_ENABLE_ASSERTS


#endif

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) std::bind(&Application::##fn, this, std::placeholders::_1)

using euint = unsigned int;
using eint = int64_t;
using euchar = unsigned char;