#pragma once
#include "formatter.hpp"

namespace DawgLog {
    class TextFormatter : public Formatter {
    public:
        /**
         * @brief Formats a log record into a text-based string representation
         *
         * This function takes a Record object and formats it into a human-readable
         * text string that includes the application name, timestamp, tag, log level,
         * message, and source location information.
         *
         * The formatted output follows this pattern:
         * "APP_NAME TIMESTAMP [TAG] LEVEL: MESSAGE, SOURCE: FILE:LINE"
         *
         * Example output: "MyApp 14:30:45 [ERROR] ERROR: Database connection failed, SOURCE: main.cpp:42"
         *
         * @param r The Record object containing all log information to format
         * @return std::string Formatted text string representation of the log record
         */
        std::string format(const Record &r) override;
    };
} // namespace DawgLog
