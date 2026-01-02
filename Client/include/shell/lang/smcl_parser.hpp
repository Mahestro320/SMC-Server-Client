#pragma once

#include <cstdint>
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>

#include "shell/lang.hpp"
#include "shell/lang/smcl_token.hpp"

namespace token_id {

extern std::unordered_map<TokenId, std::string> token_names;

extern const std::string& getName(TokenId id);

} // namespace token_id

namespace header_binding_flags {

enum HeaderBindingFlags : uint8_t {
    Name = 0b10000000,
    Locale = 0b01000000,
    Version = 0b00100000,
    Authors = 0b00010000
};

} // namespace header_binding_flags

class SMCLLexer;

class SMCLParser final {
  private:
    SMCLLexer* lexer{};

    uint8_t header_defined_bindings{};
    size_t scope_index{};
    std::vector<std::string> curr_sections{};
    std::unordered_map<size_t, std::string> curr_strbegins{};
    LangData* data{};

    bool is_in_header{};
    bool is_header_defined{};
    bool is_eof{};

    Token curr_token{};
    size_t token_idx{};

    bool checkIOValidity() const;

    bool processNextToken();
    bool getNextToken();

    bool processNewSection();
    bool processNewBinding();
    bool processNewEnd();
    bool processNewStrBegin();
    bool processNewHeader();

    bool getIdentifierName(std::string& name, TokenId end_token = TokenId::Semicolon);
    bool checkHeaderBindings() const;

    bool pushGlobalBindingValueFromKey(const std::string& key);
    bool pushHeaderBindingValueFromKey(const std::string& key);

    bool pushHeaderNameValue();
    bool pushHeaderLocaleValue();
    bool pushHeaderVersionValue();
    bool pushHeaderAuthorsValue();

    bool loadStringValue(std::string& value, std::initializer_list<TokenId> end_tokens = {TokenId::Semicolon});
    bool loadListOfStringValue(std::vector<std::string>& list,
                               std::initializer_list<TokenId> end_tokens = {TokenId::Semicolon});

    bool expectsTokens(std::initializer_list<TokenId> tokens);
    bool mustFinishWithSemicolon();

    std::string getSectionsFullName() const;
    std::string getFullStrBegins() const;

  public:
    SMCLParser(SMCLLexer* lexer);

    void setData(LangData* data);

    bool parse();
};
