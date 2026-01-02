#include "util/resolver/con_str_prop.hpp"

#include <ostream>
#include <string>

#include "util/termcolor_map.hpp"

void ConStrPropResolver::setText(const std::string& text) {
    this->text = text;
}

void ConStrPropResolver::setStream(std::ostream* stream) {
    this->stream = stream;
}

void ConStrPropResolver::print() {
    for (char c : text) {
        processChar(c);
    }
}

void ConStrPropResolver::processChar(char c) {
    if (c == '<') {
        is_in_prop = true;
        return;
    } else if (c == '>') {
        is_in_prop = false;
        processRawProp();
        return;
    }
    if (is_in_prop) {
        current_prop += c;
    } else {
        *stream << c;
    }
}

void ConStrPropResolver::processRawProp() {
    if (current_prop.empty() || !current_prop.starts_with(":")) {
        return;
    }
    current_prop = current_prop.substr(1);
    processProp();
}

void ConStrPropResolver::processProp() {
    std::string key{}, val{};
    bool is_after_equal{};
    for (char c : current_prop) {
        if (!is_after_equal && c == '=') {
            is_after_equal = true;
            continue;
        }
        if (is_after_equal) {
            val += c;
        } else {
            key += c;
        }
    }
    if (!key.empty() && !val.empty()) {
        processProp(key, val);
    }
    current_prop.clear();
}

void ConStrPropResolver::processProp(const std::string& key, const std::string& val) {
    if (key == "color") {
        setColor(val);
    }
}

void ConStrPropResolver::setColor(const std::string& color) {
    if (termcolor_map.contains(color)) {
        *stream << termcolor_map.at(color);
    }
}
