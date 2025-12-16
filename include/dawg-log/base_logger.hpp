#pragma once
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <fmt/core.h>
#include "config.hpp"
#include "sinks/sink.hpp"
#include "formatters/formatter.hpp"
#include "record.hpp"
#include "src_location.hpp"

namespace DawgLog {
   /**
    * @brief Main logging class responsible for managing log output and formatting
    *
    * The Logger class is the primary interface for generating log messages in the
    * DawgLog library. It manages a sink (where logs are written) and a formatter
    * (how logs are formatted), and provides methods to generate log records at
    * various severity levels.
    *
    * Logger instances are thread-safe and can be safely used from multiple threads.
    * The class follows a singleton pattern with the `instance()` method for accessing
    * the global logger instance.
    */
   class Logger {
   public:
    /**
     * @brief Construct a new Logger instance
     *
     * Creates a logger with specified sink, formatter, and application name.
     *
     * @param sink The sink to which log records will be written
     * @param fmt The formatter used to format log records
     * @param app_name Name of the application using this logger
     */
    Logger(SinkPtr sink, FormatterPtr fmt, std::string app_name);

    /** Deleted copy constructor - Logger is not copyable */
    Logger(const Logger &) = delete;

    /** Deleted assignment operator - Logger is not copyable */
    Logger &operator=(const Logger &) = delete;

    /**
     * @brief Log a message at the specified level with formatting support
     *
     * This templated method allows for formatted logging using fmt library syntax.
     * The format string and arguments are processed to create the final log message,
     * which is then wrapped in a Record and passed to the configured sink.
     *
     * @tparam Args Template parameters for variadic arguments
     * @param lvl The severity level of this log message
     * @param tag Optional tag for categorizing the log message
     * @param src Source location information where the log was generated
     * @param fmt_str Format string using fmt library syntax
     * @param args Arguments to be formatted into the message
     *
     * @return formatted string (the message)
     */
    template<typename... Args>
    std::string log(LogLevel lvl, std::string_view tag, const SourceLocation &src,
             fmt::string_view fmt_str, Args &&... args) {
        std::string msg;
    #if FMT_VERSION >= 80000
                 msg = fmt::format(fmt::runtime(fmt_str), std::forward<Args>(args)...);
    #else
        msg = fmt::format(fmt_str, std::forward<Args>(args)...);
    #endif
        auto rec = Record{lvl, tag, src, this->app_name_, msg};
        sink_->write(rec, formatter_->format(rec));
        return msg;
    }

    /**
     * @brief Initialize the global logger instance with configuration
     *
     * Sets up the global logger using the provided configuration. This method
     * should be called once during application startup to configure logging.
     *
     * @param cfg Configuration object containing logger settings
     */
    static void init(const Config &cfg);

    /**
     * @brief Initialize the global logger with custom formatter
     *
     * Sets up the global logger with specified configuration and custom formatter.
     *
     * @param cfg Configuration object containing logger settings
     * @param formatter Custom formatter to use for log formatting
     */
    static void init(const Config &cfg, FormatterPtr formatter);

    /**
     * @brief Initialize the global logger with custom sink
     *
     * Sets up the global logger with specified configuration and custom sink.
     *
     * @param cfg Configuration object containing logger settings
     * @param sink Custom sink to use for log output
     */
    static void init(const Config &cfg, SinkPtr sink);

    /**
     * @brief Initialize the global logger with custom sink and formatter
     *
     * Sets up the global logger with specified configuration, custom sink, and
     * custom formatter.
     *
     * @param cfg Configuration object containing logger settings
     * @param sink Custom sink to use for log output
     * @param formatter Custom formatter to use for log formatting
     */
    static void init(const Config &cfg, SinkPtr sink, FormatterPtr formatter);

    /**
     * @brief Get the global logger instance
     *
     * Returns a reference to the singleton logger instance. This should be used
     * to access the main logging interface in your application.
     *
     * @return Reference to the global Logger instance
     */
    static Logger &instance();

    /**
     * @brief Set a new formatter for this logger
     *
     * Replaces the current formatter with a new one. This operation is thread-safe.
     *
     * @param fmt New formatter to use for log formatting
     */
    void set_formatter(FormatterPtr fmt);

    /**
     * @brief Set a new sink for this logger
     *
     * Replaces the current sink with a new one. This operation is thread-safe.
     *
     * @param sink New sink to use for log output
     */
    void set_sink(SinkPtr sink);

   private:
    SinkPtr sink_;
    FormatterPtr formatter_;
    std::mutex m_;
    std::string app_name_;
   };
} // namespace DawgLog
