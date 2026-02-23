#pragma once

#include "base_logger.hpp"
#include "concepts.hpp"

#include <fmt/core.h>
#include <stdexcept>

namespace DawgLog {
/**
 * @brief Log a message with general tag for all type of logs
 * @tparam Args Variadic template parameters for formatting arguments
 * @param src Source location information for the log call
 * @param fmt_str Format string for the log message
 * @param args Arguments to format into the message
 */
#define X(name, general, str, syslog)                                                       \
    template<typename... Args>                                                              \
    static void name(const SourceLocation& src, fmt::string_view fmt_str, Args&&... args) { \
        Logger::instance().log(LogLevel::name,                                              \
                               "General",                                                   \
                               src,                                                         \
                               fmt_str,                                                     \
                               std::forward<Args>(args)...);                                \
    }
LOG_LEVELS_XMACRO
#undef X

template<ExceptionType E, typename... Args>
static void throw_error(const SourceLocation& src, fmt::string_view fmt_str, Args&&... args) {
    auto error_msg = Logger::instance().log(LogLevel::error,
                                            "General",
                                            src,
                                            fmt_str,
                                            std::forward<Args>(args)...);
    throw E{error_msg};
}

#define DEBUG(...) debug(LOG_SRC, __VA_ARGS__)

#define INFO(...) info(LOG_SRC, __VA_ARGS__)

#define NOTICE(...) notice(LOG_SRC, __VA_ARGS__)

#define WARNING(...) warning(LOG_SRC, __VA_ARGS__)

#define ERROR(...) error(LOG_SRC, __VA_ARGS__)

#define CRITICAL(...) critical(LOG_SRC, __VA_ARGS__)

#define ADD_METRIC(...) DawgLog::Logger::instance().add_metric(__VA_ARGS__)

#define METRIC(...) DawgLog::Logger::instance().report_metric(__VA_ARGS__)

#define THROW_ERROR(...) throw_error<std::runtime_error>(LOG_SRC, __VA_ARGS__)

#define TAG_DEBUG(logger, ...) (logger).debug(LOG_SRC, __VA_ARGS__)

#define TAG_INFO(logger, ...) (logger).info(LOG_SRC, __VA_ARGS__)

#define TAG_NOTICE(logger, ...) (logger).notice(LOG_SRC, __VA_ARGS__)

#define TAG_WARNING(logger, ...) (logger).warning(LOG_SRC, __VA_ARGS__)

#define TAG_ERROR(logger, ...) (logger).error(LOG_SRC, __VA_ARGS__)

#define TAG_CRITICAL(logger, ...) (logger).critical(LOG_SRC, __VA_ARGS__)

#define TAG_THROW_ERROR(logger, ...) (logger).throw_error<std::runtime_error>(LOG_SRC, __VA_ARGS__)

}  // namespace DawgLog
