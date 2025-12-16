#pragma once
#include <string>
#include <map>

namespace DawgLog {
    enum class SinkType {
        CONSOLE,
        SYSLOG
    };

    enum class FormatterType {
        JSON,
        TEXT
    };

    /**
     * @brief Creates a formatted timestamp string in HH:MM:SS format
     *
     * This function generates a current timestamp using the system clock,
     * converts it to local time, and formats it as a 24-hour time string.
     * Thread-safe implementation that handles both Windows and Unix-like systems
     * through conditional compilation.
     *
     * @return std::string Formatted timestamp in "HH:MM:SS" format
     */
    std::string make_timestamp();

    /**
     * @brief Gets the static mapping of sink type strings to SinkType enum values
     *
     * This function returns a constant reference to a map that associates string
     * representations of sink types with their corresponding enum values. The mapping
     * includes "console" -> CONSOLE and "syslog" -> SYSLOG.
     *
     * @return const std::map<std::string, SinkType>& Reference to the sink type mapping
     */
    const std::map<std::string, SinkType> &get_sink_type();

    /**
     * @brief Gets the static mapping of formatter type strings to FormatterType enum values
     *
     * This function returns a constant reference to a map that associates string
     * representations of formatter types with their corresponding enum values. The mapping
     * includes "text" -> TEXT and "json" -> JSON.
     *
     * @return const std::map<std::string, FormatterType>& Reference to the formatter type mapping
     */
    const std::map<std::string, FormatterType> &get_formatter_type();

    /**
     * @brief Converts a string representation to a SinkType enum value
     *
     * This function looks up the provided string in the sink type mapping and returns
     * the corresponding SinkType enum value. If the string is not found, it will result
     * in undefined behavior as there's no error handling for invalid input.
     *
     * @param type The string representation of the sink type to convert
     * @return SinkType The corresponding SinkType enum value
     */
    SinkType string_to_sink_type(const std::string &type);

    /**
     * @brief Converts a string representation to a FormatterType enum value
     *
     * This function looks up the provided string in the formatter type mapping and returns
     * the corresponding FormatterType enum value. If the string is not found, it will result
     * in undefined behavior as there's no error handling for invalid input.
     *
     * @param type The string representation of the formatter type to convert
     * @return FormatterType The corresponding FormatterType enum value
     */
    FormatterType string_to_formatter_type(const std::string &type);
} // namespace DawgLog
