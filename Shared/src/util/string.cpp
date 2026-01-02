#include "Shared/util/string.hpp"

#include <Windows.h>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <format>
#include <sstream>
#include <string>
#include <vector>

void util::string::toLowercase(std::string& string) {
    for (char& c : string) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
}

std::string util::string::makeFilledString(uint64_t size, char fill_char) {
    std::string result{};
    for (uint32_t i{0}; i < size; ++i) {
        result += fill_char;
    }
    return result;
}

std::string util::string::secondsToDHMS(uint64_t secs) {
    const uint64_t days{secs / 86400};
    secs %= 86400;
    const uint64_t hours{secs / 3600};
    secs %= 3600;
    const uint64_t minutes{secs / 60};
    const uint64_t seconds{secs % 60};
    return std::format("{}:{:02}:{:02}:{:02}", days, hours, minutes, seconds);
}

size_t util::string::utf8Length(const std::string& str) {
    size_t count{};
    for (size_t i = 0; i < str.size();) {
        const uint8_t c{static_cast<uint8_t>(str[i])};
        if ((c & 0x80) == 0x00) {
            i += 1;
        } else if ((c & 0xE0) == 0xC0) {
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            i += 4;
        } else {
            i += 1;
        }
        ++count;
    }
    return count;
}

std::string util::string::formatFloat(float value, uint64_t precision) {
    return std::format("{:.{}f}", value, precision);
}

std::string util::string::formatDigits(int number, uint16_t digits) {
    std::vector<char> buffer(digits + 1);
    snprintf(buffer.data(), buffer.size(), "%0*d", digits, number);
    return std::string(buffer.data());
}

std::string util::string::ptrToString(const void* ptr) {
    std::ostringstream oss;
    oss << ptr;
    return "0x" + oss.str();
}

std::string util::string::getStringFromArray(const char* array, size_t size) {
    size_t end_offset{size};
    for (size_t i{0}; i < size; ++i) {
        if (array[i] == 0x00) {
            end_offset = i;
            break;
        }
    }
    return std::string{array, end_offset};
}

std::wstring util::string::stringToWString(const std::string& str) {
    if (str.empty()) {
        return std::wstring{};
    }

    int size_needed{MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0)};
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &wstr[0], size_needed);

    return wstr;
}

std::vector<size_t> util::string::findOccurences(const std::string& str, char chr_to_find) {
    std::vector<size_t> occurences{};
    for (size_t i{}; i < str.size(); ++i) {
        if (str[i] == chr_to_find) {
            occurences.push_back(i);
        }
    }
    return occurences;
}
