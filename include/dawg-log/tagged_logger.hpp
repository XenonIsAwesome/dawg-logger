#pragma once
#include "base_logger.hpp"
#include "concepts.hpp"
#include "level.hpp"

#include <string>

namespace DawgLog {
/**
 * @brief A tagged logger class that associates log messages with a specific tag
 *
 * The TaggedLogger provides a way to categorize log messages by associating them
 * with a descriptive tag. This allows for easier filtering and organization of logs.
 *
 * Usage example:
 * ```cpp
 * TaggedLogger logger("Network");
 * logger.info(source_location, "Connection established to {}", host);
 * ```
 */
class TaggedLogger {
public:
    /**
     * @brief Construct a new TaggedLogger with the specified tag
     * @param tag The tag to associate with this logger instance
     */
    explicit TaggedLogger(std::string tag) : tag_(std::move(tag)) {}

    /**
     * @brief Log a message at the specified level
     * @tparam Args Variadic template parameters for formatting arguments
     * @param src Source location information for the log call
     * @param fmt_str Format string for the log message
     * @param args Arguments to format into the message
     */
#define X(name, general, str, syslog)                                                            \
    template<typename... Args>                                                                   \
    void name(const SourceLocation& src, fmt::string_view fmt_str, Args&&... args) {             \
        Logger::instance().log(LogLevel::name, tag_, src, fmt_str, std::forward<Args>(args)...); \
    }
    LOG_LEVELS_XMACRO
#undef X

    template<ExceptionType E, typename... Args>
    void throw_error(const SourceLocation& src, fmt::string_view fmt_str, Args&&... args) {
        auto error_msg = Logger::instance().log(LogLevel::error,
                                                tag_,
                                                src,
                                                fmt_str,
                                                std::forward<Args>(args)...);
        throw E{error_msg};
    }

    /**
     * @brief Get the tag associated with this logger
     * @return const std::string& Reference to the tag string
     */
    [[nodiscard]] const std::string& tag() const {
        return tag_;
    }

private:
    std::string tag_;
};
}  // namespace DawgLog
