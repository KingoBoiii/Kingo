#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Kingo {

	class KINGO_API Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define KE_CORE_TRACE(...) ::Kingo::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KE_CORE_INFO(...)  ::Kingo::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KE_CORE_WARN(...)  ::Kingo::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KE_CORE_ERROR(...) ::Kingo::Log::GetCoreLogger()->error(__VA_ARGS__)
#define KE_CORE_FATAL(...) ::Kingo::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define KE_INFO(...)       ::Kingo::Log::GetClientLogger()->info(__VA_ARGS__)
#define KE_TRACE(...)      ::Kingo::Log::GetClientLogger()->trace(__VA_ARGS__)
#define KE_WARN(...)       ::Kingo::Log::GetClientLogger()->warn(__VA_ARGS__)
#define KE_ERROR(...)      ::Kingo::Log::GetClientLogger()->error(__VA_ARGS__)
#define KE_FATAL(...)      ::Kingo::Log::GetClientLogger()->fatal(__VA_ARGS__)
