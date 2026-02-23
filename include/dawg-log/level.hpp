#pragma once
#include <string>
#include <syslog.h>

namespace DawgLog {
/**
 * @brief Macro for defining log levels with their string representations and syslog values
 *
 * This macro defines the standard logging levels used throughout the application.
 * Each level includes:
 * - A C++ enum value (name)
 * - A string representation (str)
 * - A corresponding syslog priority constant (syslog)
 */
#define LOG_LEVELS_XMACRO                    \
    X(debug, DEBUG, "DEBUG", LOG_DEBUG)      \
    X(info, INFO, "INFO", LOG_INFO)          \
    X(notice, NOTICE, "NOTICE", LOG_NOTICE)  \
    X(warning, WARNING, "WARN", LOG_WARNING) \
    X(error, ERROR, "ERROR", LOG_ERR)        \
    X(critical, CRITICAL, "CRITICAL", LOG_CRIT)

/**
 * @brief Enum class representing different logging levels
 *
 * Defines the hierarchical logging levels from most verbose to least verbose:
 * - debug: Detailed information for diagnosing problems
 * - info: General information about program execution
 * - warning: Something unexpected but not necessarily wrong
 * - error: A more serious problem that prevents some functionality
 * - critical: A serious error that may require immediate attention
 */
enum class LogLevel {
#define X(name, general, str, syslog) name,
    LOG_LEVELS_XMACRO
#undef X
};

/**
 * @brief Convert a LogLevel enum value to its string representation
 *
 * Converts the given log level to its corresponding string representation.
 * This is useful for formatting log messages or displaying log levels in text output.
 *
 * @param log_level The LogLevel enum value to convert
 * @return std::string String representation of the log level
 */
inline std::string to_string(LogLevel log_level) {
    switch (log_level) {
#define X(name, general, str, syslog) \
    case LogLevel::name:              \
        return str;
        LOG_LEVELS_XMACRO
#undef X
    }
    return "INFO";  // Default fallback
}

/**
 * @brief Convert a LogLevel enum value to its corresponding syslog priority level
 *
 * Converts the given log level to the appropriate syslog priority constant
 * that can be used with system logging functions like syslog().
 *
 * @param log_level The LogLevel enum value to convert
 * @return int Syslog priority constant
 */
inline int to_syslog_level(LogLevel log_level) {
    switch (log_level) {
#define X(name, general, str, syslog) \
    case LogLevel::name:              \
        return syslog;
        LOG_LEVELS_XMACRO
#undef X
    }
    return LOG_INFO;  // Default fallback
}
}  // namespace DawgLog
