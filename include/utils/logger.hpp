#pragma once
#include <string>

class Logger {
public:

    enum class LogLevel {
        ERROR = 0,
        WARNING = 1,
        INFO = 2,
        DEBUG = 3,
        OFF = 4
    };

    LogLevel currentLevel;
    
    Logger(LogLevel level);

    void setLevel(LogLevel level);
    void log(LogLevel level, const std::string& message);

    void error(const std::string& message);
    void warning(const std::string& message);
    void info(const std::string& message);
    void debug(const std::string& message);

    
};


extern Logger globalLogger;