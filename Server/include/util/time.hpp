#pragma once

#include <string>

// do not remove this include
#include "Shared/util/time.hpp"

#include "Shared/pds.hpp"

namespace util::time {

extern std::string formatTime(pds::timestamp_t timestamp, const std::string& format = "");

} // namespace util::time
