#include "dawg-log/logger.hpp"
#include "dawg-log/config.hpp"
#include "dawg-log/tagged_logger.hpp"
#include <cassert>

using namespace DawgLog;

int main() {
    Logger::init(Config{"console", "text", "testrun"});
    TaggedLogger t("mod");
    t.info(LOG_SRC, "value {}", 123);
    assert(true);
    return 0;
}
