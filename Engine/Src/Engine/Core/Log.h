#pragma once

#include "Engine/Core/Core.h"
#include "spdlog/spdlog.h"

namespace ee
{
	
	class Log
	{
		
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	};

}

// Core Logging Macros
#define EE_CORE_TRACE(...)		ee::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EE_CORE_INFO(...)		ee::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EE_CORE_WARN(...)		ee::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EE_CORE_ERROR(...)		ee::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EE_CORE_CRITICAL(...)	ee::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client Logging Macros
#define EE_TRACE(...)		ee::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EE_INFO(...)		ee::Log::GetClientLogger()->info(__VA_ARGS__)
#define EE_WARN(...)		ee::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EE_ERROR(...)		ee::Log::GetClientLogger()->error(__VA_ARGS__)
#define EE_CRITICAL(...)	ee::Log::GetClientLogger()->critical(__VA_ARGS__) 