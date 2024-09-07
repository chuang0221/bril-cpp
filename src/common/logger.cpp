#include "common/logger.h"
#include <nlohmann/json.hpp>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::setLogLevel(LogLevel level) {
    currentLevel = level;
}

void Logger::log(LogLevel level, const std::string& message) {
    if (level >= currentLevel) {
        std::cout << getTimestamp() << " [" << getLevelString(level) << "] " << message << '\n';
    }
}

template<typename T>
void Logger::logJson(LogLevel level, const std::string& message, const T& json) {
    if (level >= currentLevel) {
        std::cout << getTimestamp() << " [" << getLevelString(level) << "] " << message << " ";
        std::cout << json.dump(2) << '\n';
    }
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Logger::getLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

template void Logger::logJson<nlohmann::json>(LogLevel level, const std::string& message, const nlohmann::json& json);