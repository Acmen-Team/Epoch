#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Epoch {

  class Log
  {
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
#define EP_CORE_TRACE(...)		::Epoch::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define EP_CORE_INFO(...)		::Epoch::Log::GetCoreLogger()->info(__VA_ARGS__);
#define EP_CORE_WARN(...)		::Epoch::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define EP_CORE_ERROR(...)		::Epoch::Log::GetCoreLogger()->error(__VA_ARGS__);
#define EP_CORE_FATAL(...)		::Epoch::Log::GetCoreLogger()->fatal(__VA_ARGS__);