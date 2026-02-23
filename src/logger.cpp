#include "dawg-log/base_logger.hpp"
#include "dawg-log/concepts.hpp"
#include "dawg-log/formatters/json_formatter.hpp"
#include "dawg-log/formatters/text_formatter.hpp"
#include "dawg-log/general_logs.hpp"
#include "dawg-log/sinks/console_sink.hpp"
#include "dawg-log/sinks/file_sink.hpp"
#include "dawg-log/sinks/syslog_sink.hpp"

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

SinkPtr make_sink(const SinkType type, const std::string& app_name, const std::string& file_path) {
    switch (type) {
        case SinkType::SYSLOG:
            return std::make_unique<SyslogSink>(app_name);
        case SinkType::FILE:
            return std::make_unique<FileSink>(file_path);
        default:
            return std::make_unique<ConsoleSink>(app_name);
    }
}

Logger::Target make_target(SinkType sink_type,
                           FormatterType formatter_type,
                           const std::string& app_name,
                           const std::string& file_path) {
    return Logger::Target{make_sink(sink_type, app_name, file_path),
                          make_formatter(formatter_type)};
}

std::vector<Logger::Target> make_targets_from_config(const Config& cfg) {
    std::vector<Logger::Target> targets;
    if (!cfg.targets.empty()) {
        targets.reserve(cfg.targets.size());
        for (const auto& target : cfg.targets) {
            targets.emplace_back(
                make_target(target.sink, target.format, cfg.app_name, target.file_path));
        }
        return targets;
    }
    targets.emplace_back(make_target(cfg.sink, cfg.format, cfg.app_name, cfg.file_path));
    return targets;
}
}  // namespace

Logger::Logger(std::vector<Target> targets, std::string app_name) :
    targets_(std::move(targets)), app_name_(std::move(app_name)) {}

void Logger::init(const Config& cfg) {
    logger = std::make_unique<Logger>(make_targets_from_config(cfg), cfg.app_name);
}

void Logger::init(const Config& cfg, FormatterPtr formatter) {
    std::vector<Target> targets;
    targets.emplace_back(
        Target{make_sink(cfg.sink, cfg.app_name, cfg.file_path), std::move(formatter)});
    logger = std::make_unique<Logger>(std::move(targets), cfg.app_name);
}

void Logger::init(const Config& cfg, SinkPtr sink) {
    std::vector<Target> targets;
    targets.emplace_back(Target{std::move(sink), make_formatter(cfg.format)});
    logger = std::make_unique<Logger>(std::move(targets), cfg.app_name);
}

void Logger::init(const Config& cfg, SinkPtr sink, FormatterPtr formatter) {
    std::vector<Target> targets;
    targets.emplace_back(Target{std::move(sink), std::move(formatter)});
    logger = std::make_unique<Logger>(std::move(targets), cfg.app_name);
}

void Logger::init(const Config& cfg, std::vector<Target> targets) {
    logger = std::make_unique<Logger>(std::move(targets), cfg.app_name);
}

Logger& Logger::instance() {
    if (!logger) {
        std::vector<Target> targets;
        targets.emplace_back(
            make_target(SinkType::CONSOLE, FormatterType::TEXT, "DawgLog", "dawglog.log"));
        logger = std::make_unique<Logger>(std::move(targets), "DawgLog");
        WARNING("Logger not initialized. Defaulting to console sink and text format.");
    }
    return *logger;
}

void Logger::set_formatter(FormatterPtr fmt) {
    std::lock_guard<std::mutex> lock(m_);
    if (targets_.empty()) {
        return;
    }
    targets_.front().formatter = std::move(fmt);
}

void Logger::set_sink(SinkPtr sink) {
    std::lock_guard<std::mutex> lock(m_);
    if (targets_.empty()) {
        return;
    }
    targets_.front().sink = std::move(sink);
}

void Logger::set_targets(std::vector<Target> targets) {
    std::lock_guard<std::mutex> lock(m_);
    targets_ = std::move(targets);
}

void Logger::add_target(SinkPtr sink, FormatterPtr formatter) {
    std::lock_guard<std::mutex> lock(m_);
    targets_.push_back(Target{std::move(sink), std::move(formatter)});
}
