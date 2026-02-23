#include "dawg-log/sinks/console_sink.hpp"

#include <iostream>

using namespace DawgLog;

void ConsoleSink::write(const Record& r, std::string_view formatted) {
    std::lock_guard lock(m_);
    if (r.level >= LogLevel::warning) {
        std::cerr << formatted << '\n';
        std::cerr.flush();
    } else {
        std::cout << formatted << '\n';
        std::cout.flush();
    }
}