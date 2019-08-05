#include "eepch.h"

#define EE_BUILD_ID "v0.1a"

namespace ee
{
	void InitializeCore()
	{

		ee::Log::Init();

		EE_CORE_TRACE("Sravan Engine {}", EE_BUILD_ID);
		EE_CORE_TRACE("Initializing...");

	}

	void ShutdownCore()
	{
		
		EE_CORE_TRACE("Shutting down...");

	}
}
