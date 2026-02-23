#pragma once
#include "formatter.hpp"

namespace DawgLog {
/**
 * @brief JSON formatter for log records
 *
 * This class implements the Formatter interface to provide JSON-formatted log output.
 * It converts Record objects into structured JSON strings that include:
 * - Timestamp information
 * - Log level
 * - Message content
 * - Any additional fields or metadata from the record
 *
 * The formatted output follows a consistent JSON structure suitable for machine processing
 * and integration with logging systems that consume JSON-formatted logs.
 */
class JsonFormatter : public Formatter {
public:
    /**
     * @brief Format a log record as a JSON string
     *
     * Takes a Record object and converts it into a JSON-formatted string representation.
     * The output includes all relevant information from the record in a structured format.
     *
     * @param r The log record to format
     * @return std::string JSON formatted string representing the log record
     */
    std::string format(const Record& r) override;
};
}  // namespace DawgLog
