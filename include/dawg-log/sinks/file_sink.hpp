#pragma once
#include "sink.hpp"

#include <fstream>
#include <mutex>
#include <string>

namespace DawgLog {
/**
 * @brief File sink implementation for logging to a file
 *
 * The FileSink class writes formatted log records to a file in a thread-safe manner.
 */
class FileSink : public Sink {
public:
    explicit FileSink(std::string path);

    void write(const Record& r, std::string_view formatted) override;

private:
    std::string path_;
    std::ofstream out_;
    std::mutex m_;
};
}  // namespace DawgLog
