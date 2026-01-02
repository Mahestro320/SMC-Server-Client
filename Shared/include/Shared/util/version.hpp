#pragma once

#include <cstdint>
#include <string>

#include "Shared/pds.hpp"

struct Version final {
    uint16_t major{}, minor{}, patch{};

    std::string toString(char delim = '.') const;
    pds::version_t combine() const;

    void setFromCombined(pds::version_t combined);
    bool setFromString(const std::string& str);
};
