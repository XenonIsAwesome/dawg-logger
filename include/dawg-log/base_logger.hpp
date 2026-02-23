#pragma once
#include "config.hpp"
#include "formatters/formatter.hpp"
#include "record.hpp"
#include "sinks/sink.hpp"
#include "src_location.hpp"

#include <fmt/core.h>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#if LOGGERLIB_HAS_METRICS
#    include <prometheus/histogram.h>
#    include <prometheus/registry.h>
#    include <prometheus/summary.h>
#endif

namespace DawgLog {
#if LOGGERLIB_HAS_METRICS
static prometheus::Histogram::BucketBoundaries DAWGLOG_DEFAULT_BUCKETS =
    {0.005, 0.01, 0.025, 0.05, 0.1, 0.25, 0.5, 1, 2.5, 5, 10};

static prometheus::Summary::Quantiles DAWGLOG_DEFAULT_QUANTILES = {{0.5, 0.05},
                                                                   {0.9, 0.01},
                                                                   {0.99, 0.001}};
#endif

/**
 * @brief Main logging class responsible for managing log output and formatting
 *
 * The Logger class is the primary interface for generating log messages in the
 * DawgLog library. It manages a sink (where logs are written) and a formatter
 * (how logs are formatted), and provides methods to generate log records at
 * various severity levels.
 *
 * Logger instances are thread-safe and can be safely used from multiple threads.
 * The class follows a singleton pattern with the `instance()` method for accessing
 * the global logger instance.
 */
class Logger {
public:
    struct Target {
        SinkPtr sink;
        FormatterPtr formatter;
    };
    /**
     * @brief Construct a new Logger instance
     *
     * Creates a logger with specified sink, formatter, and application name.
     *
     * @param sink The sink to which log records will be written
     * @param fmt The formatter used to format log records
     * @param app_name Name of the application using this logger
     */
    Logger(std::vector<Target> targets, std::string app_name);

    /** Deleted copy constructor - Logger is not copyable */
    Logger(const Logger&) = delete;

    /** Deleted assignment operator - Logger is not copyable */
    Logger& operator=(const Logger&) = delete;

    /**
     * @brief Log a message at the specified level with formatting support
     *
     * This templated method allows for formatted logging using fmt library syntax.
     * The format string and arguments are processed to create the final log message,
     * which is then wrapped in a Record and passed to the configured sink.
     *
     * @tparam Args Template parameters for variadic arguments
     * @param lvl The severity level of this log message
     * @param tag Optional tag for categorizing the log message
     * @param src Source location information where the log was generated
     * @param fmt_str Format string using fmt library syntax
     * @param args Arguments to be formatted into the message
     *
     * @return formatted string (the message)
     */
    template<typename... Args>
    std::string log(LogLevel lvl,
                    std::string_view tag,
                    const SourceLocation& src,
                    fmt::string_view fmt_str,
                    Args&&... args) {
        std::lock_guard<std::mutex> lock(m_);
        std::string msg;
#if FMT_VERSION >= 80000
        msg = fmt::format(fmt::runtime(fmt_str), std::forward<Args>(args)...);
#else
        msg = fmt::format(fmt_str, std::forward<Args>(args)...);
#endif
        auto rec = Record{lvl, tag, src, this->app_name_, msg};
        for (auto& target : targets_) {
            if (!target.sink || !target.formatter) {
                continue;
            }
            target.sink->write(rec, target.formatter->format(rec));
        }
        return msg;
    }

    /**
     * @brief Initialize the global logger instance with configuration
     *
     * Sets up the global logger using the provided configuration. This method
     * should be called once during application startup to configure logging.
     *
     * @param cfg Configuration object containing logger settings
     */
    static void init(const Config& cfg);

    /**
     * @brief Initialize the global logger with custom formatter
     *
     * Sets up the global logger with specified configuration and custom formatter.
     *
     * @param cfg Configuration object containing logger settings
     * @param formatter Custom formatter to use for log formatting
     */
    static void init(const Config& cfg, FormatterPtr formatter);

    /**
     * @brief Initialize the global logger with custom sink
     *
     * Sets up the global logger with specified configuration and custom sink.
     *
     * @param cfg Configuration object containing logger settings
     * @param sink Custom sink to use for log output
     */
    static void init(const Config& cfg, SinkPtr sink);

    /**
     * @brief Initialize the global logger with custom sink and formatter
     *
     * Sets up the global logger with specified configuration, custom sink, and
     * custom formatter.
     *
     * @param cfg Configuration object containing logger settings
     * @param sink Custom sink to use for log output
     * @param formatter Custom formatter to use for log formatting
     */
    static void init(const Config& cfg, SinkPtr sink, FormatterPtr formatter);

    /**
     * @brief Initialize the global logger with a set of sink/formatter targets
     *
     * @param cfg Configuration object containing logger settings
     * @param targets Set of sink/formatter pairs to receive each log record
     */
    static void init(const Config& cfg, std::vector<Target> targets);

    /**
     * @brief Get the global logger instance
     *
     * Returns a reference to the singleton logger instance. This should be used
     * to access the main logging interface in your application.
     *
     * @return Reference to the global Logger instance
     */
    static Logger& instance();

    /**
     * @brief Set a new formatter for this logger
     *
     * Replaces the current formatter with a new one. This operation is thread-safe.
     *
     * @param fmt New formatter to use for log formatting
     */
    void set_formatter(FormatterPtr fmt);

    /**
     * @brief Set a new sink for this logger
     *
     * Replaces the current sink with a new one. This operation is thread-safe.
     *
     * @param sink New sink to use for log output
     */
    void set_sink(SinkPtr sink);

    /**
     * @brief Replace all logging targets with a new set
     *
     * @param targets New sink/formatter targets to use
     */
    void set_targets(std::vector<Target> targets);

    /**
     * @brief Add a new sink/formatter target
     *
     * @param sink New sink to receive log records
     * @param formatter Formatter used for this sink
     */
    void add_target(SinkPtr sink, FormatterPtr formatter);

#if LOGGERLIB_HAS_METRICS
    using Labels = std::map<std::string, std::string>;

    using FamilyVariant = std::variant<prometheus::Family<prometheus::Counter>*,
                                       prometheus::Family<prometheus::Gauge>*,
                                       prometheus::Family<prometheus::Histogram>*,
                                       prometheus::Family<prometheus::Summary>*>;

    enum class MetricAction { Increment, Decrement, Set, Observe };
    /**
     * @return Returns the logger's prometheus registry
     */
    std::shared_ptr<prometheus::Registry> registry();

    // type: "counter" | "gauge" | "histogram" | "summary"
    // For histogram, provide bucket boundaries (optional, has sane default)
    // For summary, provide quantiles (optional, has sane default)
    void add_metric(const std::string& name,
                    const std::string& help,
                    prometheus::MetricType type,
                    prometheus::Histogram::BucketBoundaries buckets = DAWGLOG_DEFAULT_BUCKETS,
                    prometheus::Summary::Quantiles quantiles = DAWGLOG_DEFAULT_QUANTILES) {
        std::lock_guard lock(m_);

        if (families_.count(name))
            throw std::runtime_error("Metric already registered: " + name);

        switch (type) {
            case prometheus::MetricType::Counter: {
                families_[name] = &prometheus::BuildCounter().Name(name).Help(help).Register(
                    *_prometheus_registry);
                break;
            }
            case prometheus::MetricType::Gauge: {
                families_[name] =
                    &prometheus::BuildGauge().Name(name).Help(help).Register(*_prometheus_registry);
                break;
            }
            case prometheus::MetricType::Summary: {
                auto& family = prometheus::BuildSummary().Name(name).Help(help).Register(
                    *_prometheus_registry);
                summary_quantiles_[name] = std::move(quantiles);
                families_[name] = &family;
                break;
            }
            case prometheus::MetricType::Untyped:
                throw std::invalid_argument("Unsupported metric type");
            case prometheus::MetricType::Histogram: {
                // Buckets are fixed per-family, not per label set
                auto& family = prometheus::BuildHistogram().Name(name).Help(help).Register(
                    *_prometheus_registry);
                histogram_buckets_[name] = std::move(buckets);
                families_[name] = &family;
                break;
            }
            case prometheus::MetricType::Info:
                throw std::invalid_argument("Unsupported metric type");
        }
    }

    void add_metric(const std::string& name, prometheus::MetricType type);

    void report_metric(const std::string& name,
                       MetricAction action,
                       double value = 1.0,
                       const Labels& labels = {}) {
        std::lock_guard lock(m_);

        auto it = families_.find(name);
        if (it == families_.end())
            throw std::runtime_error("Unknown metric: " + name);

        std::visit(
            [&](auto* family) {
                using FamilyT = std::decay_t<decltype(*family)>;

                if constexpr (std::is_same_v<FamilyT, prometheus::Family<prometheus::Counter>>) {
                    if (action == MetricAction::Increment)
                        family->Add(labels).Increment(value);
                    else
                        throw std::invalid_argument("Counter only supports Increment");

                } else if constexpr (std::is_same_v<FamilyT,
                                                    prometheus::Family<prometheus::Gauge>>) {
                    auto& gauge = family->Add(labels);
                    if (action == MetricAction::Increment)
                        gauge.Increment(value);
                    else if (action == MetricAction::Decrement)
                        gauge.Decrement(value);
                    else if (action == MetricAction::Set)
                        gauge.Set(value);
                    else
                        throw std::invalid_argument("Invalid action for Gauge");

                } else if constexpr (std::is_same_v<FamilyT,
                                                    prometheus::Family<prometheus::Histogram>>) {
                    if (action == MetricAction::Observe)
                        family->Add(labels, histogram_buckets_.at(name)).Observe(value);
                    else
                        throw std::invalid_argument("Histogram only supports Observe");

                } else if constexpr (std::is_same_v<FamilyT,
                                                    prometheus::Family<prometheus::Summary>>) {
                    if (action == MetricAction::Observe)
                        family->Add(labels, summary_quantiles_.at(name)).Observe(value);
                    else
                        throw std::invalid_argument("Summary only supports Observe");
                }
            },
            it->second);
    }
#endif

private:
    std::vector<Target> targets_;
    std::mutex m_;
    std::string app_name_;

#if LOGGERLIB_HAS_METRICS
    std::shared_ptr<prometheus::Registry> _prometheus_registry;
    std::unordered_map<std::string, FamilyVariant> families_;
    std::unordered_map<std::string, prometheus::Histogram::BucketBoundaries> histogram_buckets_;
    std::unordered_map<std::string, prometheus::Summary::Quantiles> summary_quantiles_;
#endif
};
}  // namespace DawgLog
