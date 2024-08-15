#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


#define LOGGER_TRACE(...)  Logger::GetLogger()->trace(__VA_ARGS__)
#define LOGGER_INFO(...)   Logger::GetLogger()->info(__VA_ARGS__)
#define LOGGER_WARN(...)   Logger::GetLogger()->warn(__VA_ARGS__)
#define LOGGER_ERROR(...)  Logger::GetLogger()->error(__VA_ARGS__)
#define LOGGER_FATAL(...)  Logger::GetLogger()->critical(__VA_ARGS__)

class Logger {
private:
	static std::shared_ptr<spdlog::logger> s_Logger;

public:	
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; };
};