#include <dawg-log/logger.hpp>
#include <filesystem>
#include <string>

#if LOGGERLIB_HAS_METRICS
#include <prometheus/text_serializer.h>
#endif

namespace dog = DawgLog;

int main(int argc, char** argv) {
    dog::Config cfg{"../config.multi.json"};
    dog::Logger::init(cfg);

    dog::TaggedLogger ingest("ingest");

    ingest.info(LOG_SRC, "ingest step {}", 1);

#if LOGGERLIB_HAS_METRICS
    ADD_METRIC("metric1", "metric1", prometheus::MetricType::Counter);
    for (size_t i = 0; i < 10; i++) {
        METRIC("metric1", DawgLog::Logger::MetricAction::Increment, i + 1);
    }

    const auto serializer = prometheus::TextSerializer{};
    const auto metrics = DawgLog::Logger::instance().registry()->Collect();
    std::cout << serializer.Serialize(metrics) << std::endl;
#endif

    // try {
    //     throw std::runtime_error("Blah");
    // }
    // catch(std::exception& e) {
    //     // THROW_ERROR("noder {}", 1);
    //     ingest.throw_error<std::runtime_error>(LOG_SRC, "noder check {}", 32);
    // }
    //
    // // dog::Logger::instance().set_formatter(std::make_unique<dog::JsonFormatter>());
    // ingest.notice(LOG_SRC, "now json format!");
    //
    // TAG_DEBUG(ingest ,"asdada {}", 1);
    // NOTICE("asdada {}", 1);

    return 0;
}
