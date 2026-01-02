#pragma once

#include "Shared/pds.hpp"

namespace util::time {

extern pds::timestamp_t getSeconds();
extern pds::timestamp_t getMillis();
extern pds::timestamp_t getNanos();

} // namespace util::time
