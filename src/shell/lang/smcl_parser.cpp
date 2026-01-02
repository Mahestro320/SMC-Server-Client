#include "shell/lang/smcl_parser.hpp"

#include <algorithm>
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/util/version.hpp"
#include "shell/lang.hpp"
#include "shell/lang/smcl_lexer.hpp"
#include "shell/lang/smcl_token.hpp"

namespace {

void printError(const std::string& error) {
    console::out::err("[SMCL parser] error: " + error);
}

void printVerbose(const std::string& inf) {
    console::out::verbose("[SMCL parser] " + inf);
}

} // namespace

std::unordered_map<TokenId, std::string> token_id::token_names{
    {TokenId::Dot, "dot"},
    {TokenId::Semicolon, "semicolon"},
    {TokenId::Equal, "equal"},
    {TokenId::OpeningBracket, "opening bracket"},
    {TokenId::ClosingBracket, "closing bracket"},
    {TokenId::Comma, "comma"},

    {TokenId::Identifier, "identifier"},
    {TokenId::String, "string"},
    {TokenId::Number, "number"},

    {TokenId::Section, "section"},
    {TokenId::StrBegin, "strbegin"},
    {TokenId::Header, "header"},
    {TokenId::End, "end"},

    {TokenId::_EOF, "eof"},
};

const std::string& token_id::getName(TokenId id) {
    return ((token_names.contains(id)) ? token_names.at(id) : lang::UNKNOWN);
}

SMCLParser::SMCLParser(SMCLLexer* lexer) : lexer{lexer} {
}

void SMCLParser::setData(LangData* data) {
    this->data = data;
}

bool SMCLParser::parse() {
    if (!checkIOValidity()) {
        return false;
    }
    while (!is_eof) {
        if (!processNextToken()) {
            return false;
        }
    }
    if (scope_index != 0) {
        printError("missing end of block at end");
        return false;
    }
    if (!is_header_defined) {
        printError("header is not defined");
        return false;
    }
    return true;
}

bool SMCLParser::checkIOValidity() const {
    if (!lexer) {
        printError("lexer is nullptr");
        return false;
    }
    if (!data) {
        printError("output data is nullptr");
        return false;
    }
    return true;
}

bool SMCLParser::processNextToken() {
    if (!getNextToken()) {
        return false;
    }
    if (curr_token.id == TokenId::_EOF) {
        is_eof = true;
        return true;
    } else if (curr_token.id == TokenId::Semicolon) {
        return true;
    } else if (curr_token.id == TokenId::Dot) {
        return processNewBinding();
    } else if (curr_token.id == TokenId::Section) {
        return processNewSection();
    } else if (curr_token.id == TokenId::StrBegin) {
        return processNewStrBegin();
    } else if (curr_token.id == TokenId::Header) {
        return processNewHeader();
    } else if (curr_token.id == TokenId::End) {
        return processNewEnd();
    }
    printError("invalid symbol: " + token_id::getName(curr_token.id) + " \"" + curr_token.contentToString() + '\"');
    return false;
}

bool SMCLParser::processNewSection() {
    if (is_in_header) {
        printError("header can't contain sections");
        return false;
    }
    std::string section_name{};
    if (!getIdentifierName(section_name)) {
        return false;
    }
    scope_index++;
    curr_sections.push_back(section_name);
    return true;
}

bool SMCLParser::processNewBinding() {
    std::string binding_name{};
    if (!getIdentifierName(binding_name, TokenId::Equal)) {
        return false;
    }
    const std::string key{getSectionsFullName() + binding_name};
    return ((is_in_header) ? pushHeaderBindingValueFromKey(key) : pushGlobalBindingValueFromKey(key));
}

bool SMCLParser::pushGlobalBindingValueFromKey(const std::string& key) {
    std::string value{};
    if (!loadStringValue(value)) {
        return false;
    }
    printVerbose("loaded global binding: " + key);
    data->translations.insert({key, value});
    return true;
}

bool SMCLParser::pushHeaderBindingValueFromKey(const std::string& key) {
    if (key == "name") {
        return pushHeaderNameValue();
    } else if (key == "locale") {
        return pushHeaderLocaleValue();
    } else if (key == "version") {
        return pushHeaderVersionValue();
    } else if (key == "authors") {
        return pushHeaderAuthorsValue();
    }
    printError("unknown header binding: " + key);
    return false;
}

bool SMCLParser::pushHeaderNameValue() {
    if (header_defined_bindings & header_binding_flags::Name) {
        printError("lang name is already defined");
        return false;
    }
    header_defined_bindings |= header_binding_flags::Name;
    std::string value{};
    if (!loadStringValue(value)) {
        return false;
    }
    printVerbose("lang name: " + value);
    data->name = value;
    return true;
}

bool SMCLParser::pushHeaderLocaleValue() {
    if (header_defined_bindings & header_binding_flags::Locale) {
        printError("lang locale is already defined");
        return false;
    }
    header_defined_bindings |= header_binding_flags::Locale;
    std::string value{};
    if (!loadStringValue(value)) {
        return false;
    }
    printVerbose("lang locale: " + value);
    data->locale = value;
    return true;
}

bool SMCLParser::pushHeaderVersionValue() {
    if (header_defined_bindings & header_binding_flags::Version) {
        printError("lang version is already defined");
        return false;
    }
    header_defined_bindings |= header_binding_flags::Version;
    Version value{};
    if (!expectsTokens({TokenId::Number})) {
        return false;
    }
    value.major = get<int>(curr_token.content);
    if (!expectsTokens({TokenId::Dot}) || !expectsTokens({TokenId::Number})) {
        return false;
    }
    value.minor = get<int>(curr_token.content);
    if (!expectsTokens({TokenId::Dot}) || !expectsTokens({TokenId::Number})) {
        return false;
    }
    value.patch = get<int>(curr_token.content);
    printVerbose("lang version: " + value.toString());
    data->version = value;
    return true;
}

bool SMCLParser::pushHeaderAuthorsValue() {
    if (header_defined_bindings & header_binding_flags::Authors) {
        printError("lang authors are already defined");
        return false;
    }
    header_defined_bindings |= header_binding_flags::Authors;
    std::vector<std::string> value{};
    if (!loadListOfStringValue(value)) {
        return false;
    }
    printVerbose("lang authors: ");
    for (size_t i{}; i < value.size(); i++) {
        printVerbose("\t- " + value[i]);
    }
    data->authors = value;
    return true;
}

bool SMCLParser::loadStringValue(std::string& value, std::initializer_list<TokenId> end_tokens) {
    value += getFullStrBegins();
    bool is_end_token{};
    size_t i{};
    while (true) {
        if (!getNextToken()) {
            return false;
        }
        if (curr_token.id == TokenId::String) {
            value += curr_token.contentToString();
            i++;
            continue;
        }
        for (TokenId end_token : end_tokens) {
            if (curr_token.id == end_token) {
                is_end_token = true;
                break;
            }
        }
        if (!is_end_token || (is_end_token && i == 0)) {
            printError("strgen: expecting " + token_id::getName(TokenId::String) + ", got " +
                       token_id::getName(curr_token.id));
            return false;
        }
        if (is_end_token) {
            break;
        }
    }
    return true;
}

bool SMCLParser::loadListOfStringValue(std::vector<std::string>& list, std::initializer_list<TokenId> end_tokens) {
    if (!expectsTokens({TokenId::OpeningBracket})) {
        return false;
    }
    while (true) {
        std::string value{};
        if (!loadStringValue(value, {TokenId::Comma, TokenId::ClosingBracket})) {
            return false;
        }
        list.push_back(value);
        if (curr_token.id == TokenId::ClosingBracket) {
            break;
        }
    }
    return mustFinishWithSemicolon();
}

bool SMCLParser::processNewEnd() {
    if (is_in_header) {
        is_in_header = false;
        return checkHeaderBindings();
    }
    if (scope_index == 0) {
        printError("missing section opening");
        return false;
    }
    if (curr_strbegins.contains(scope_index)) {
        curr_strbegins.erase(scope_index);
    }
    scope_index--;
    curr_sections.pop_back();
    return mustFinishWithSemicolon();
}

bool SMCLParser::processNewStrBegin() {
    if (!expectsTokens({TokenId::String})) {
        return false;
    }
    curr_strbegins.insert({scope_index, curr_token.contentToString()});
    return mustFinishWithSemicolon();
}

bool SMCLParser::processNewHeader() {
    if (is_header_defined || is_in_header) {
        printError("header is already defined");
        return false;
    }
    if (scope_index != 0) {
        printError("header can't be defined in a block");
        return false;
    }
    is_in_header = true;
    is_header_defined = true;
    return mustFinishWithSemicolon();
}

bool SMCLParser::getIdentifierName(std::string& name, TokenId end_token) {
    bool string_started{};
    bool last_is_point{};
    while (true) {
        if (!getNextToken()) {
            return false;
        }
        if (curr_token.id == end_token) {
            if (last_is_point) {
                printError("found point at identifier name ending");
                return false;
            }
            if (string_started) {
                return true;
            }
            printError("missing identifier name");
            return false;
        } else if (curr_token.id == TokenId::Dot) {
            last_is_point = true;
            if (string_started) {
                name += simple_chars::DOT;
                continue;
            }
            printError("found point at identifier name beginning");
            return false;
        } else if (curr_token.id == TokenId::Identifier) {
            name += curr_token.contentToString();
            string_started = true;
        } else {
            printError("unexpected token: " + curr_token.contentToString());
            return false;
        }
        last_is_point = false;
    }
    return true;
}

bool SMCLParser::checkHeaderBindings() const {
    bool is_error{};
    if (!(header_defined_bindings & header_binding_flags::Name)) {
        printError("missing header binding 'name'");
        is_error = true;
    }
    if (!(header_defined_bindings & header_binding_flags::Locale)) {
        printError("missing header binding 'locale'");
        is_error = true;
    }
    if (!(header_defined_bindings & header_binding_flags::Version)) {
        printError("missing header binding 'version'");
        is_error = true;
    }
    if (!(header_defined_bindings & header_binding_flags::Authors)) {
        printError("missing header binding 'authors'");
        is_error = true;
    }
    return !is_error;
}

bool SMCLParser::expectsTokens(std::initializer_list<TokenId> tokens) {
    if (!getNextToken()) {
        return false;
    }
    for (TokenId token : tokens) {
        if (curr_token.id == token) {
            return true;
        }
    }
    std::string tokens_message{};
    const size_t tokens_size{tokens.size()};
    for (size_t i{}; i < tokens.size(); ++i) {
        tokens_message += token_id::getName(*(tokens.begin() + i));
        if (tokens_size != 1 && i < tokens_size - 2) {
            tokens_message += ", ";
        } else if (i < tokens_size - 1) {
            tokens_message += " or ";
        }
    }
    printError("expecting " + tokens_message + ", got " + token_id::getName(curr_token.id));
    return false;
}

bool SMCLParser::mustFinishWithSemicolon() {
    if (!getNextToken()) {
        return false;
    }
    if (curr_token.id != TokenId::Semicolon) {
        printError("missing ';' at end of expression");
        return false;
    }
    return true;
}

bool SMCLParser::getNextToken() {
    curr_token = lexer->nextToken();
    if (lexer->isError()) {
        return false;
    }
    return true;
}

std::string SMCLParser::getSectionsFullName() const {
    std::string full_name{};
    for (size_t i{}; i < curr_sections.size(); ++i) {
        full_name += curr_sections[i] + '.';
    }
    return full_name;
}

std::string SMCLParser::getFullStrBegins() const {
    std::vector<size_t> keys{};
    keys.reserve(curr_strbegins.size());
    for (const auto& [key, _] : curr_strbegins) {
        keys.push_back(key);
    }
    std::sort(keys.begin(), keys.end());

    std::string result{};
    for (size_t key : keys) {
        const auto it = curr_strbegins.find(key);
        if (it != curr_strbegins.end()) {
            result += it->second;
        }
    }
    return result;
}
