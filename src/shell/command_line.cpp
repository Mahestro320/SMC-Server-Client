#include "shell/command_line.hpp"

#include <cctype>
#include <string>
#include <vector>

#include "Shared/io/console.hpp"
#include "util/env_var_str_resolver.hpp"

bool CommandLine::getLine() {
    std::string line{console::in::getLine()};
    EnvVarStrResolver resolver{};
    resolver.setInput(line);
    resolver.resolve();
    raw_line = resolver.getOutput();
    return true;
}

void CommandLine::tokenize() {
    for (char c : raw_line) {
        if (processSimpleQuotes(c) || processDoubleQuotes(c)) {
            last = c;
            continue;
        }
        if (is_in_simple_quotes || is_in_double_quotes || !std::isspace(static_cast<unsigned char>(c))) {
            current_item += c;
        } else {
            flushToken(current_item);
        }
        last = c;
    }
    flushToken(current_item);
}

bool CommandLine::processSimpleQuotes(char c) {
    if (c != '\'' || is_in_double_quotes) {
        return false;
    }
    is_in_simple_quotes = !is_in_simple_quotes;
    if (last == '\'') {
        tokenized_line.push_back("");
    } else {
        flushToken(current_item);
    }
    return true;
}

bool CommandLine::processDoubleQuotes(char c) {
    if (c != '"' || is_in_simple_quotes) {
        return false;
    }
    is_in_double_quotes = !is_in_double_quotes;
    if (last == '"') {
        tokenized_line.push_back("");
    } else {
        flushToken(current_item);
    }
    return true;
}

void CommandLine::flushToken(std::string& item) {
    if (!item.empty()) {
        tokenized_line.push_back(item);
        item.clear();
    }
}

bool CommandLine::empty() const {
    return raw_line.empty();
}

const std::vector<std::string>& CommandLine::getTokens() const {
    return tokenized_line;
}
