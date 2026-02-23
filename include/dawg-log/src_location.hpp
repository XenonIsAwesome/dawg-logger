#pragma once

#define LOG_SRC                      \
    ::DawgLog::SourceLocation {      \
        __FILE__, __LINE__, __func__ \
    }

namespace DawgLog {

struct SourceLocation {
    const char* file{""};
    int line{0};
    const char* func{""};
};

}  // namespace DawgLog
