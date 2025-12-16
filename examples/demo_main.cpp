#include <dawg-log/logger.hpp>

namespace dog = DawgLog;

int main(int argc, char** argv) {

    dog::Config cfg{"/home/user/projects/dawg-logger/config.json"};
    dog::Logger::init(cfg);

    dog::TaggedLogger ingest("ingest");

    ingest.info(LOG_SRC, "ingest step {}", 1);
    try {
        throw std::runtime_error("Blah");
    }
    catch(std::exception& e) {
        // THROW_ERROR("noder {}", 1);
        ingest.throw_error<std::runtime_error>(LOG_SRC, "noder check {}", 32);
    }

    // dog::Logger::instance().set_formatter(std::make_unique<dog::JsonFormatter>());
    ingest.notice(LOG_SRC, "now json format!");

    TAG_DEBUG(ingest ,"asdada {}", 1);
    NOTICE("asdada {}", 1);

    return 0;
}
