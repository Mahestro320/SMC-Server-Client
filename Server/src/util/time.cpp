#include "util/time.hpp"

#include <chrono>
#include <format>
#include <string>

#include "Shared/pds.hpp"
#include "server/config.hpp"

std::string util::time::formatTime(pds::timestamp_t timestamp, const std::string& format) {
    const std::chrono::sys_seconds tp{std::chrono::seconds{timestamp}};
    if (!format.empty()) {
        return std::vformat(format, std::make_format_args(tp));
    }
    Config& config{Config::getInstance()};
    const ConfigValues& values{config.getValues()};
    return std::vformat(values.time_format, std::make_format_args(tp));
}
