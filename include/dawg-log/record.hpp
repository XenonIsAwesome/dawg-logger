#pragma once
#include "level.hpp"
#include "src_location.hpp"
#include "utils.hpp"

#include <string>

namespace DawgLog {
/**
 * @brief Represents a single log record containing all information about a log entry
 *
 * A Record encapsulates all the data associated with a log message, including:
 * - Application name and timestamp
 * - Log level and tag
 * - The actual log message content
 * - Source location information where the log was generated
 * - Thread ID for multithreaded applications
 *
 * Records are typically created by Logger instances and passed to Sinks for output.
 */
struct Record {
    /** Name of the application that generated this log record */
    std::string app_name;

    /** formatted timestamp when the record was created */
    std::string timestamp;

    /** Log level indicating the severity of the message */
    LogLevel level{LogLevel::info};

    /** Optional tag for categorizing log messages */
    std::string tag;

    /** The actual log message content */
    std::string message;

    /** Source location information where the log was generated */
    SourceLocation src;

    /**
     * @brief Construct a new Record instance
     *
     * Creates a log record with all necessary information. Automatically generates
     * timestamp and thread ID, and initializes other fields from parameters.
     *
     * @param lvl The log level of this record
     * @param tag Optional tag for categorizing the log message
     * @param src Source location where the log was generated
     * @param app_name Name of the application generating the log
     * @param msg The actual log message content
     */
    Record(LogLevel lvl,
           std::string_view tag,
           const SourceLocation& src,
           std::string_view app_name,
           std::string_view msg) :
        app_name(app_name), timestamp(make_timestamp()), level(lvl), tag(tag), message(msg),
        src(src) {}
};
}  // namespace DawgLog
