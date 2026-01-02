#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "concepts.hpp"

namespace util::string {

static constexpr char CHAR_SPACE{' '};
static constexpr char CHAR_HASHTAG{'#'};
static constexpr char CHAR_MINUS{'-'};
static constexpr char CHAR_SLASH{'/'};

extern void toLowercase(std::string& string);
extern std::string makeFilledString(uint64_t size, char fill_char = ' ');
extern std::string secondsToDHMS(uint64_t secs);
extern std::string formatFloat(float value, uint64_t precision = 1U);
extern size_t utf8Length(const std::string& str);
extern std::string ptrToString(const void* ptr);
extern std::string getStringFromArray(const char* array, size_t size);
extern std::string formatDigits(int number, uint16_t digits);
extern std::wstring stringToWString(const std::string& str);
extern std::vector<size_t> findOccurences(const std::string& str, char chr_to_find);

template<is_arithmetic T> bool stoi(const std::string& str, T& out);

} // namespace util::string

#include "string.ipp"
