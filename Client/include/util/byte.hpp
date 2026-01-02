#pragma once

#include <cstdint>
#include <string>

#include "Shared/util/concepts.hpp"

namespace util::byte {

template<is_integral T = uint64_t> std::string toAutoUnit(T num) {
    if (num < 0x400) {
        return std::to_string(num) + " b";
    } else if (num < 0x100000) {
        return std::to_string(num / 0x400) + " Kb";
    } else if (num < 0x40000000) {
        return std::to_string(num / 0x100000) + " Mb";
    } else if (num < 0x10000000000) {
        return std::to_string(num / 0x40000000) + " Gb";
    } else if (num < 0x4000000000000) {
        return std::to_string(num / 0x10000000000) + " Tb";
    }
    return std::to_string(num) + " b";
}

} // namespace util::byte
