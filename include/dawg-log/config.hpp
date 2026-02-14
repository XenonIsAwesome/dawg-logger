#pragma once
#include "sinks/sink.hpp"
#include "formatters/formatter.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>

namespace DawgLog {
    /**
     * @brief Configuration class for logger settings
     *
     * The Config class manages all logger configuration parameters that can be loaded
     * from a JSON file. It provides default values when the configuration file is
     * missing or invalid, ensuring the logger always has valid settings.
     *
     * Configuration options include:
     * - Sink type (console, file, etc.)
     * - Formatter type (text, JSON, etc.)
     * - Application name for log identification
     *
     * The class automatically handles file I/O errors and provides sensible defaults
     * when configuration values are missing or invalid.
     */
    class Config {
    public:
        struct TargetConfig {
            SinkType sink{SinkType::CONSOLE};
            FormatterType format{FormatterType::TEXT};
            std::string file_path{"dawglog.log"};
        };
        /**
         * @brief Logger sink type enumeration
         *
         * Defines the destination where log messages will be written.
         * Supported sinks include console output and file output.
         */
        SinkType sink;

        /**
         * @brief Logger formatter type enumeration
         *
         * Defines the format used for logging messages.
         * Supported formats include text and JSON formatting.
         */
        FormatterType format;

        /**
         * @brief Application name for log identification
         *
         * Used to identify the source application in log entries.
         * This name appears in formatted log output to help distinguish
         * between different applications or services.
         */
        std::string app_name;
        std::string file_path;
        std::vector<TargetConfig> targets;

        /**
         * @brief Construct a Config object from JSON file
         *
         * Loads configuration settings from a JSON file. If the file cannot be opened
         * or parsed, default values are used instead:
         * - Sink: Console output
         * - Format: Text formatting
         * - App name: "DawgLog"
         *
         * @param json_path Path to the JSON configuration file
         */
        explicit Config(const std::string &json_path) {
            const auto resolve_path = [](const std::string &raw_path) {
                std::string expanded_path = raw_path;
                if (!raw_path.empty() && raw_path[0] == '~') {
                    const char* home = std::getenv("HOME");
                    if (home != nullptr) {
                        if (raw_path.size() == 1) {
                            expanded_path = home;
                        } else if (raw_path[1] == '/') {
                            expanded_path = std::string(home) + raw_path.substr(1);
                        }
                    }
                }

                const std::filesystem::path path{expanded_path};
                if (path.is_absolute()) {
                    return path.lexically_normal().string();
                }
                return path.lexically_normal().string();
            };

            std::ifstream file(json_path);
            if (!file.is_open()) {
                std::cerr << "Failed to open logger config file: " << json_path << std::endl;
                std::cout << "Set's logger to default settings: (sink = console, format = text, app_name = DawgLog)" <<
                        std::endl;
                sink = SinkType::CONSOLE;
                format = FormatterType::TEXT;
                app_name = "DawgLog";
                file_path = "dawglog.log";
                return;
            }

            nlohmann::json j;
            file >> j;

            sink = string_to_sink_type(j.value("sink", "console"));
            format = string_to_formatter_type(j.value("format", "text"));
            app_name = j.value("app_name", "DawgLog");
            file_path = resolve_path(j.value("file_path", "dawglog.log"));

            if (j.contains("targets") && j["targets"].is_array()) {
                for (const auto &target : j["targets"]) {
                    if (!target.is_object()) {
                        continue;
                    }
                    TargetConfig cfg;
                    cfg.sink = string_to_sink_type(target.value("sink", "console"));
                    cfg.format = string_to_formatter_type(target.value("format", "text"));
                    cfg.file_path = resolve_path(target.value("file_path", "dawglog.log"));
                    targets.emplace_back(std::move(cfg));
                }
            }
        }
    };
} // namespace DawgLog
