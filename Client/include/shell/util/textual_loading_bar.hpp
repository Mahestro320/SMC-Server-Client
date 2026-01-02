#pragma once

#include <cstdint>
#include <string>

#include "Shared/util/concepts.hpp"
#include "util/string.hpp"

template<is_integral StoredValueType> class TextualLoadingBar final {
  private:
    static inline constexpr uint16_t BORDER_SIZE{2};

    const char fill_char{}, empty_char{};
    const uint16_t visual_size{};

    StoredValueType value{}, maximum{};

  public:
    TextualLoadingBar() = delete;
    TextualLoadingBar(uint16_t visual_size, char fill_char = util::string::CHAR_HASHTAG,
                      char empty_char = util::string::CHAR_MINUS)
        : visual_size{static_cast<uint16_t>(visual_size - BORDER_SIZE)}, fill_char{fill_char}, empty_char{empty_char} {
    }

    void setValue(StoredValueType value) {
        this->value = std::min(value, maximum);
    }

    void setMaximum(StoredValueType maximum) {
        this->maximum = maximum;
    }

    void increment() {
        value = std::min(static_cast<StoredValueType>(value + 1), maximum);
    }

    void decrement() {
        value = (value == 0) ? 0 : value - 1;
    }

    std::string getText() const {
        std::string bar_content{};
        const uint64_t fill_size{
            static_cast<uint64_t>(static_cast<float>(value) * (static_cast<float>(visual_size) / maximum))};
        bar_content += util::string::makeFilledString(fill_size, fill_char);
        bar_content += util::string::makeFilledString(visual_size - fill_size, empty_char);
        return '[' + bar_content + ']';
    }
};
