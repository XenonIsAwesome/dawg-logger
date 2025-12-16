#pragma once
#include "../record.hpp"
#include <memory>
#include <string_view>

namespace DawgLog {
    /**
     * @brief Abstract base class for log sinks
     *
     * A Sink is responsible for writing formatted log records to some output destination.
     * This virtual base class defines the interface that all concrete sink implementations
     * must follow. Sinks are typically used by Logger instances to actually output log messages.
     *
     * @note All derived classes must implement the write() method to handle the actual
     *       output logic for formatted log records.
     */
    class Sink {
    public:
        /** Virtual destructor for proper inheritance cleanup */
        virtual ~Sink() = default;

        /**
         * @brief Write a formatted log record to the sink's destination
         *
         * This method is called by Logger instances to output formatted log messages.
         * The implementation should handle writing the formatted string to the appropriate
         * output destination (file, console, network, etc.).
         *
         * @param r The original log record that was formatted
         * @param formatted The pre-formatted string representation of the log record
         */
        virtual void write(const Record &r, std::string_view formatted) = 0;
    };

    /** Type alias for unique pointer to Sink */
    using SinkPtr = std::unique_ptr<Sink>;
} // namespace DawgLog
