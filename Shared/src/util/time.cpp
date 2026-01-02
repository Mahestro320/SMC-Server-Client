#include "Shared/util/time.hpp"

#include <chrono>
#include <type_traits>

#include "Shared/pds.hpp"

namespace {

template<typename duration, std::enable_if_t<std::chrono::_Is_duration_v<duration>, bool> = true>
pds::timestamp_t getTimestamp() {
    const auto now{std::chrono::system_clock::now()};
    const auto time{std::chrono::duration_cast<duration>(now.time_since_epoch())};
    return static_cast<pds::timestamp_t>(time.count());
}

} // namespace

pds::timestamp_t util::time::getSeconds() {
    return getTimestamp<std::chrono::seconds>();
}

pds::timestamp_t util::time::getMillis() {
    return getTimestamp<std::chrono::milliseconds>();
}

pds::timestamp_t util::time::getNanos() {
    return getTimestamp<std::chrono::nanoseconds>();
}
