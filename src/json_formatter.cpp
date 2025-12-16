#include "dawg-log/formatters/json_formatter.hpp"
#include <nlohmann/json.hpp>

using namespace DawgLog;

std::string JsonFormatter::format(const Record& r) {
    nlohmann::json j;
    j["app_name"] = r.app_name;
    j["time"] = r.timestamp;
    j["level"] = std::string(to_string(r.level));
    j["tag"] = r.tag;
    j["message"] = r.message;

    return j.dump();
}