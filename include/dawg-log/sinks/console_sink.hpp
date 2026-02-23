#pragma once
#include "sink.hpp"

#include <mutex>
#include <utility>

namespace DawgLog {
/**
 * @brief Console sink implementation for logging to standard output
 *
 * The ConsoleSink class is a concrete implementation of the Sink interface
 * that writes log records to the console (standard output). It provides thread-safe
 * writing operations through mutex protection and includes application name
 * formatting in log messages.
 *
 * This sink is useful for development, debugging, and situations where logs
 * need to be displayed directly in the terminal or command prompt. The console
 * output can be easily redirected or piped to other programs.
 */
class ConsoleSink : public Sink {
public:
    /**
     * @brief Construct a new ConsoleSink object
     *
     * Creates a console sink with the specified application name that will be
     * included in all log messages written by this sink.
     *
     * @param app_name The name of the application to include in log output
     */
    explicit ConsoleSink(std::string app_name) : app_name(std::move(app_name)) {}

    /**
     * @brief Write a formatted log record to console
     *
     * Writes the formatted log message to standard output in a thread-safe manner.
     * The method acquires a mutex lock before writing to ensure that concurrent
     * logging operations do not interleave output.
     *
     * @param r The log record containing metadata about the log entry
     * @param formatted The pre-formatted string representation of the log message
     */
    void write(const Record& r, std::string_view formatted) override;

private:
    std::string app_name;
    std::mutex m_;
};
}  // namespace DawgLog
