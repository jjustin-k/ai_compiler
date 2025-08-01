#include "../include/utils/logger.hpp"

Logger::Logger(LogLevel level) : currentLevel(level) {}

void Logger::setLevel(LogLevel level) { currentLevel = level; }

// Define global instance
Logger globalLogger(Logger::LogLevel::INFO);
