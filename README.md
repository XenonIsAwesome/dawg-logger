# DawgLogger 🐾
A lightweight and flexible C++ logging library with **tagged loggers**, **configurable sinks**, and **JSON/Text output formats**, built on top of [`fmt`](https://github.com/fmtlib/fmt) and [`nlohmann/json`](https://github.com/nlohmann/json).

---

## ✨ Features
- Simple configuration via JSON file
- Supports **console** and **syslog** sinks
- Supports **text** and **JSON** formatting
- Tagged loggers for module-specific logging
- Multiple log levels: `debug`, `info`, `warn`, `error`, `critical`
- Customizable formatters with `Logger::instance().set_formatter(...)`

---

## 📦 Dependencies
DawgLogger requires the following libraries:

# fmt
```bash
  sudo apt install libfmt-dev
```

# nlohmann_json
- [nlohmann/json](https://github.com/nlohmann/json)

Both can be installed via package managers (`apt`, `vcpkg`, `conan`) or manually.

---

## ⚙️ Building & Installing

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
sudo cmake --build build --target install -- -j$(nproc)
```

### ✅ Tests

Tests are built by default. You can disable them with:

```bash
cmake -B build -S . -DDAWGLOG_BUILD_TESTS=OFF
```

To run tests:

```bash
cmake --build build -j
ctest --test-dir build
```

---

## 🔗 Using DawgLogger in your project

In your `CMakeLists.txt`:

```cmake
find_package(DawgLogger REQUIRED)
find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE DawgLog::dawg-logger)
```

Include the logger:

```cpp
#include <DawgLogger/logger.hpp>
```

---

## 📝 Configuration

DawgLogger is initialized from a JSON config file that defines:
- `app_name` – name of your application
- `format` – output format (`text` or `json`)
- `sink` – logging sink (`console` or `syslog`)

**Example config.json:**
```json
{
  "app_name": "MyApp",
  "format": "text",
  "sink": "console"
}
```

### Multiple targets (sink + formatter pairs)

You can send different formats to different sinks by defining `targets`:

```json
{
  "app_name": "MyApp",
  "targets": [
    { "sink": "syslog", "format": "json" },
    { "sink": "console", "format": "text" }
  ]
}
```

Then initialize the logger using these targets:

```cpp
#include <DawgLogger/logger.hpp>

namespace dog = DawgLog;

int main() {
    dog::Config cfg{"config.json"};
    std::vector<dog::Logger::Target> targets;
    targets.emplace_back(dog::Logger::Target{
        std::make_unique<dog::SyslogSink>(cfg.app_name),
        std::make_unique<dog::JsonFormatter>()
    });
    targets.emplace_back(dog::Logger::Target{
        std::make_unique<dog::ConsoleSink>(cfg.app_name),
        std::make_unique<dog::TextFormatter>()
    });
    dog::Logger::init(cfg, std::move(targets));
}
```

---

## 📝 Rsyslog and Logrotate installation

Run ```./install_logrotate.sh <your app's name>```

just make sure that this is the same name you put in the config.json file

---

## 🚀 Example Usage

```cpp
#include <DawgLogger/logger.hpp>

namespace dog = DawgLog;

int main() {
    dog::Config cfg{"config.json"};
    dog::Logger::init(cfg);

    // Create a tagged logger
    dog::Logger var_name("tag");

    // Log messages
    var_name.info(LOG_SRC, "hi {}", 1);

    // Change formatter at runtime
    dog::Logger::instance().set_formatter(std::make_unique<dog::JsonFormatter>());
}
```

### Example Output (text mode):
```
DawgLog 10:54:14 [tag] INFO: hi 1, SOURCE: /home/.../main.cpp:8
```

### Example Output (JSON mode):
```json
{
  "app_name": "MyApp",
  "level": "INFO",
  "message": "hi 1",
  "tag": "tag",
  "time": "10:54:14"
}
```

---

## 📖 Log Functions
- `debug`
- `info`
- `notice`
- `warn`
- `error`
- `critical`
- `throw_error`

---
