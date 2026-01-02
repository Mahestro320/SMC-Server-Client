#pragma once

#include <cstdint>
#include <string>
#include <variant>

enum class TokenId : uint8_t {
    Dot,
    Semicolon,
    Equal,
    OpeningBracket,
    ClosingBracket,
    Comma,

    Identifier,
    String,
    Number,

    Section,
    StrBegin,
    Header,
    End,

    _EOF,
};

struct Token {
    TokenId id{};
    std::variant<std::string, int> content{};

    std::string contentToString() const;
};
