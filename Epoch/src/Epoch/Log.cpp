#include "eppch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Epoch {

  std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
  std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

  void Log::Init()
  {
	spdlog::set_pattern("%^[%T] %n: %v%$");

	s_CoreLogger = spdlog::stdout_color_mt("Epoch");
	s_CoreLogger->set_level(spdlog::level::trace);
  }

}