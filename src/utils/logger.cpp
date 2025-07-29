#include "../include/utils/logger.hpp"
#include <iostream>
Logger::Logger(LogLevel level) : currentLevel(level) {}

void Logger::setLevel(LogLevel level) {
    currentLevel = level;
}

void Logger::error(const std::string& message) {
    if (currentLevel >= LogLevel::ERROR)
        std::cout << "[ERROR] " << message << std::endl;
}

void Logger::warning(const std::string& message) {
    if (currentLevel >= LogLevel::WARNING)
        std::cout << "[WARNING] " << message << std::endl;
}

void Logger::info(const std::string& message) {
    if (currentLevel >= LogLevel::INFO)
        std::cout << "[INFO] " << message << std::endl;
}

void Logger::debug(const std::string& message) {
    if (currentLevel >= LogLevel::DEBUG)
        std::cout << "[DEBUG] " << message << std::endl;
}

Logger globalLogger(Logger::LogLevel::INFO);