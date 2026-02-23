#include "dawg-log/sinks/syslog_sink.hpp"

#ifdef LOGGERLIB_HAS_SYSLOG
#    include <syslog.h>
#endif

using namespace DawgLog;

SyslogSink::SyslogSink(std::string app_name) : app_(std::move(app_name)) {
    openlog(app_.c_str(), LOG_PID | LOG_NDELAY, LOG_USER);
}

SyslogSink::~SyslogSink() {
    closelog();
}

void SyslogSink::write(const Record& r, std::string_view formatted) {
    syslog(to_syslog_level(r.level), "%s", std::string(formatted).c_str());
}