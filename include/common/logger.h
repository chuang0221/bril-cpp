#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger {
public:
    static Logger& getInstance();
    void setLogLevel(LogLevel level);
    void log(LogLevel level, const std::string& message);
    
    template<typename T>
    void logJson(LogLevel level, const std::string& message, const T& json);

private:
    Logger() = default;
    LogLevel currentLevel = LogLevel::INFO;
    std::string getTimestamp();
    std::string getLevelString(LogLevel level);
};

#define LOG_DEBUG(message) Logger::getInstance().log(LogLevel::DEBUG, message)
#define LOG_INFO(message) Logger::getInstance().log(LogLevel::INFO, message)
#define LOG_WARNING(message) Logger::getInstance().log(LogLevel::WARNING, message)
#define LOG_ERROR(message) Logger::getInstance().log(LogLevel::ERROR, message)

#define LOG_JSON_DEBUG(message, json) Logger::getInstance().logJson(LogLevel::DEBUG, message, json)
#define LOG_JSON_INFO(message, json) Logger::getInstance().logJson(LogLevel::INFO, message, json)
#define LOG_JSON_WARNING(message, json) Logger::getInstance().logJson(LogLevel::WARNING, message, json)
#define LOG_JSON_ERROR(message, json) Logger::getInstance().logJson(LogLevel::ERROR, message, json)

#endif