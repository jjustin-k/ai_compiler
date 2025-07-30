#pragma once
#include <string>
#include <iostream>

class Logger {
public:
    enum class LogLevel {
        ERROR,
        WARNING,
        INFO,
        DEBUG
    };

    Logger(LogLevel level);
    void setLevel(LogLevel level);

    template <typename T>
    void error(const T &message) {
        if (currentLevel >= LogLevel::ERROR)
            std::cout << "[ERROR] " << message << std::endl;
    }

    template <typename T>
    void warning(const T &message) {
        if (currentLevel >= LogLevel::WARNING)
            std::cout << "[WARNING] " << message << std::endl;
    }

    template <typename T>
    void info(const T &message) {
        if (currentLevel >= LogLevel::INFO)
            std::cout << "[INFO] " << message << std::endl;
    }

    template <typename T>
    void debug(const T &message) {
        if (currentLevel >= LogLevel::DEBUG)
            std::cout << "[DEBUG] " << message << std::endl;
    }


    LogLevel currentLevel;
};

// Global instance
extern Logger globalLogger;
