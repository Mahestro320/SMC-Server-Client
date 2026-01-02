#include "Shared/util/version.hpp"

#include <cstdint>
#include <string>
#include <vector>

#include "Shared/pds.hpp"
#include "Shared/util/string.hpp"

std::string Version::toString(char delim) const {
    return std::to_string(major) + delim + std::to_string(minor) + delim + std::to_string(patch);
}

pds::version_t Version::combine() const {
    return (static_cast<pds::version_t>(major) << 32) | (static_cast<pds::version_t>(minor) << 16) |
           static_cast<pds::version_t>(patch);
}

void Version::setFromCombined(pds::version_t combined) {
    major = static_cast<uint16_t>((combined >> 32) & 0xFFFF);
    minor = static_cast<uint16_t>((combined >> 16) & 0xFFFF);
    patch = static_cast<uint16_t>(combined & 0xFFFF);
}

bool Version::setFromString(const std::string& str) {
    const std::vector<size_t> occur{util::string::findOccurences(str, '.')};
    if (occur.size() != 2) {
        return false;
    }
    return util::string::stoi<uint16_t>(str.substr(0, occur[0]), major) &&
           util::string::stoi<uint16_t>(str.substr(occur[0] + 1, occur[1]), minor) &&
           util::string::stoi<uint16_t>(str.substr(occur[1] + 1, str.size()), patch);
}
