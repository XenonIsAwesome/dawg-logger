#pragma once
#include "sink.hpp"

#include <string>

namespace DawgLog {
/**
 * @brief A logging sink that writes messages to the system log (syslog)
 *
 * The SyslogSink class implements the Sink interface to direct log records
 * to the system logging facility. It uses the application name to identify
 * the source of log messages in the syslog.
 *
 * This sink is typically used on Unix-like systems where syslog is available.
 */
class SyslogSink : public Sink {
public:
    /**
     * @brief Construct a new SyslogSink with the specified application name
     * @param app_name The name to use as the application identifier in syslog
     */
    explicit SyslogSink(std::string app_name);

    /**
     * @brief Destroy the SyslogSink instance
     *
     * Cleans up any resources associated with the syslog connection.
     */
    ~SyslogSink();

    /**
     * @brief Write a formatted log record to the system log
     * @param r The log record containing metadata and level information
     * @param formatted The already-formatted log message string
     */
    void write(const Record& r, std::string_view formatted) override;

private:
    std::string app_;
};
}  // namespace DawgLog
