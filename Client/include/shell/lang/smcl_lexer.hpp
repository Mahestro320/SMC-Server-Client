#pragma once

#include <string>

#include "Shared/util/concepts.hpp"
#include "smcl_token.hpp"

namespace simple_chars {

static inline constexpr char DOT{'.'};
static inline constexpr char SEMICOLON{';'};
static inline constexpr char EQUAL{'='};
static inline constexpr char OPENING_BRACKET{'['};
static inline constexpr char CLOSING_BRACKET{']'};
static inline constexpr char COMMA{','};

} // namespace simple_chars

namespace keywords {

static inline constexpr char SECTION[]{"SECTION"};
static inline constexpr char STRBEGIN[]{"STRBEGIN"};
static inline constexpr char HEADER[]{"HEADER"};
static inline constexpr char END[]{"END"};

} // namespace keywords

class SMCLLexer final {
  private:
    const std::string* data{};

    size_t curr_pos{};
    Token curr_token{};

    bool is_error{};

    bool buildToken();
    bool buildTokenFromIdx();
    bool isSimpleCharToken(size_t idx) const;
    bool buildTokenFromChar(char ch);
    void buildTokenFromIdentifier(const std::string& str);
    bool buildTokenFromString();
    bool buildTokenFromNumber();

    size_t getStartIdx() const;
    size_t getEndIdxFromCurrPos() const;

    template<int_or_string T> void buildTokenFromIdAndContent(TokenId id, T content = T{}) {
        curr_token = Token{
            .id = id,
            .content = content,
        };
    }

    static char getEscapeChar(char esc);

  public:
    SMCLLexer(const std::string* data);

    Token nextToken();
    bool isError() const;
};
