#include "shell/lang/smcl_lexer.hpp"

#include <cctype>
#include <cstdint>
#include <string>

#include "Shared/io/console.hpp"
#include "shell/lang/smcl_token.hpp"

namespace {

void printError(const std::string& error) {
    console::out::err("[SMCL Lexer] error: " + error);
}

} // namespace

SMCLLexer::SMCLLexer(const std::string* data) : data{data} {
}

Token SMCLLexer::nextToken() {
    if (!data) {
        printError("data is nullptr");
        is_error = true;
        return {};
    }
    if (!buildToken()) {
        is_error = true;
    }
    return curr_token;
}

bool SMCLLexer::buildToken() {
    curr_token = Token{};
    const size_t start_idx{getStartIdx()};
    if (start_idx == SIZE_MAX) {
        buildTokenFromIdAndContent<std::string>(TokenId::_EOF, "EOF");
        return true;
    }
    curr_pos = start_idx;
    return buildTokenFromIdx();
}

bool SMCLLexer::buildTokenFromIdx() {
    const char ch{(*data)[curr_pos]};
    if (buildTokenFromChar(ch)) {
        return true;
    }
    if (ch == '\"') {
        return buildTokenFromString();
    }
    if (isdigit(ch)) {
        return buildTokenFromNumber();
    }
    buildTokenFromIdentifier(data->substr(curr_pos, getEndIdxFromCurrPos() - curr_pos));
    return true;
}

bool SMCLLexer::buildTokenFromChar(char ch) {
    if (ch == simple_chars::DOT) {
        buildTokenFromIdAndContent(TokenId::Dot, std::string{ch});
    } else if (ch == simple_chars::SEMICOLON) {
        buildTokenFromIdAndContent(TokenId::Semicolon, std::string{ch});
    } else if (ch == simple_chars::EQUAL) {
        buildTokenFromIdAndContent(TokenId::Equal, std::string{ch});
    } else if (ch == simple_chars::OPENING_BRACKET) {
        buildTokenFromIdAndContent(TokenId::OpeningBracket, std::string{ch});
    } else if (ch == simple_chars::CLOSING_BRACKET) {
        buildTokenFromIdAndContent(TokenId::ClosingBracket, std::string{ch});
    } else if (ch == simple_chars::COMMA) {
        buildTokenFromIdAndContent(TokenId::Comma, std::string{ch});
    } else {
        return false;
    }
    curr_pos++;
    return true;
}

bool SMCLLexer::isSimpleCharToken(size_t idx) const {
    const char ch{(*data)[idx]};
    return ch == simple_chars::DOT || ch == simple_chars::SEMICOLON || ch == simple_chars::EQUAL ||
           ch == simple_chars::OPENING_BRACKET || ch == simple_chars::CLOSING_BRACKET || ch == simple_chars::COMMA;
}

void SMCLLexer::buildTokenFromIdentifier(const std::string& str) {
    if (str == keywords::SECTION) {
        buildTokenFromIdAndContent(TokenId::Section, str);
    } else if (str == keywords::STRBEGIN) {
        buildTokenFromIdAndContent(TokenId::StrBegin, str);
    } else if (str == keywords::HEADER) {
        buildTokenFromIdAndContent(TokenId::Header, str);
    } else if (str == keywords::END) {
        buildTokenFromIdAndContent(TokenId::End, str);
    } else {
        buildTokenFromIdAndContent(TokenId::Identifier, str);
    }
    curr_pos += str.size();
}

bool SMCLLexer::buildTokenFromString() {
    std::string final_str{};
    bool escaped{};
    curr_pos++; // skip the first quote
    for (; curr_pos < data->size(); curr_pos++) {
        const char ch{(*data)[curr_pos]};
        if (escaped) {
            final_str += getEscapeChar(ch);
            escaped = false;
            continue;
        }
        if (ch == '\\') {
            escaped = true;
            continue;
        }
        if (ch == '"') {
            buildTokenFromIdAndContent(TokenId::String, final_str);
            curr_pos++;
            return true;
        }
        final_str += ch;
    }
    printError("string not closed at end");
    return false;
}

bool SMCLLexer::buildTokenFromNumber() {
    int final_num{};
    for (; curr_pos < data->size(); curr_pos++) {
        const char ch{(*data)[curr_pos]};
        if (!std::isdigit(ch)) {
            break;
        }
        int ch_int{ch - '0'};
        final_num = final_num * 10 + ch_int;
    }
    buildTokenFromIdAndContent(TokenId::Number, final_num);
    return true;
}

size_t SMCLLexer::getStartIdx() const {
    for (size_t i{curr_pos}; i < data->size(); i++) {
        if (!std::isspace((*data)[i])) {
            return i;
        }
    }
    return SIZE_MAX;
}

size_t SMCLLexer::getEndIdxFromCurrPos() const {
    size_t i{curr_pos};
    for (; i < data->size(); i++) {
        if (std::isspace((*data)[i]) || isSimpleCharToken(i)) {
            return i;
        }
    }
    return i;
}

bool SMCLLexer::isError() const {
    return is_error;
}

char SMCLLexer::getEscapeChar(char esc) {
    if (esc == 'n') {
        return '\n';
    } else if (esc == 't') {
        return '\t';
    } else if (esc == 'v') {
        return '\v';
    } else if (esc == 'f') {
        return '\f';
    } else if (esc == '\\') {
        return '\\';
    } else if (esc == '"') {
        return '\"';
    }
    return '?';
}
