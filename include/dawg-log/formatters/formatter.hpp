#pragma once
#include "../record.hpp"

#include <memory>
#include <string>

namespace DawgLog {
/**
 * @brief Base class for log record formatters
 *
 * The Formatter interface defines the contract for converting Record objects
 * into formatted string representations. Different formatter implementations
 * can provide various output formats such as JSON, plain text, or custom formats.
 *
 * This abstract base class ensures that all concrete formatter implementations
 * provide a consistent method for formatting log records while allowing flexibility
 * in the actual formatting logic.
 */
class Formatter {
public:
    /**
     * @brief Virtual destructor for proper inheritance cleanup
     *
     * Ensures that derived classes are properly destructed when deleted through
     * a base class pointer.
     */
    virtual ~Formatter() = default;

    /**
     * @brief Format a log record into a string representation
     *
     * This pure virtual method must be implemented by derived classes to convert
     * a Record object into a formatted string. The format can vary depending on
     * the specific implementation (JSON, plain text, etc.).
     *
     * @param r The log record to format
     * @return std::string Formatted string representation of the log record
     */
    virtual std::string format(const Record& r) = 0;
};

/**
 * @brief Smart pointer type alias for Formatter objects
 *
 * Provides a convenient way to manage Formatter instances with automatic memory
 * management using unique_ptr semantics. This ensures proper cleanup of formatter
 * objects without manual memory management.
 */
using FormatterPtr = std::unique_ptr<Formatter>;
}  // namespace DawgLog
