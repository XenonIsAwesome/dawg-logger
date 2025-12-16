#include "dawg-log/base_logger.hpp"
#include "dawg-log/sinks/console_sink.hpp"
#include "dawg-log/sinks/syslog_sink.hpp"
#include "dawg-log/formatters/text_formatter.hpp"
#include "dawg-log/formatters/json_formatter.hpp"

using namespace DawgLog;

namespace {
std::unique_ptr<Logger> logger;

FormatterPtr make_formatter(const FormatterType type) {
    switch (type) {
        case FormatterType::JSON:
            return std::make_unique<JsonFormatter>();
        default:
            return std::make_unique<TextFormatter>();
    }
}

SinkPtr make_sink(const SinkType type, const std::string& app_name) {
    switch (type) {
        case SinkType::SYSLOG:
            return std::make_unique<SyslogSink>(app_name);
        default:
            return std::make_unique<ConsoleSink>(app_name);
    }
}
}

Logger::Logger(SinkPtr sink, FormatterPtr formatter, std::string app_name)
    : sink_(std::move(sink)), formatter_(std::move(formatter)), app_name_(std::move(app_name)) {}

void Logger::init(const Config& cfg) {
    init(cfg, make_formatter(cfg.format));
}

void Logger::init(const Config& cfg, FormatterPtr formatter) {
    logger = std::make_unique<Logger>(make_sink(cfg.sink, cfg.app_name),
                                      std::move(formatter),
                                      cfg.app_name);
}

void Logger::init(const Config& cfg, SinkPtr sink) {
    logger = std::make_unique<Logger>(std::move(sink), make_formatter(cfg.format), cfg.app_name);
}

void Logger::init(const Config& cfg, SinkPtr sink, FormatterPtr formatter) {
    logger = std::make_unique<Logger>(std::move(sink), std::move(formatter), cfg.app_name);
}

Logger& Logger::instance() {
    return *logger;
}

void Logger::set_formatter(FormatterPtr fmt) {
    std::lock_guard<std::mutex> lock(m_);
    formatter_ = std::move(fmt);
}

void Logger::set_sink(SinkPtr sink) {
    std::lock_guard<std::mutex> lock(m_);
    sink_ = std::move(sink);
}