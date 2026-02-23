#include "dawg-log/sinks/file_sink.hpp"

#include <iostream>

using namespace DawgLog;

FileSink::FileSink(std::string path) : path_(std::move(path)), out_(path_, std::ios::app) {
    if (!out_.is_open()) {
        std::cerr << "Failed to open log file: " << path_ << std::endl;
    }
}

void FileSink::write(const Record& r, std::string_view formatted) {
    std::lock_guard lock(m_);
    if (!out_.is_open()) {
        return;
    }
    out_ << formatted << '\n';
    out_.flush();
}
