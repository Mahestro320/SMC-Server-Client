#include "util/env_var_str_resolver.hpp"

#include <cstdlib>
#include <malloc.h>
#include <string>

void EnvVarStrResolver::resolve() {
    for (char c : input) {
        processNextChar(c);
    }
    if (is_in_var) {
        output += '<' + current_var;
    }
}

void EnvVarStrResolver::processNextChar(char c) {
    if (c == '<' && !is_in_var) {
        is_in_var = true;
        return;
    }
    if (c == '>' && is_in_var) {
        is_in_var = false;
        insertEnvVar();
        return;
    }
    if (is_in_var) {
        current_var += c;
    } else {
        output += c;
    }
}

void EnvVarStrResolver::insertEnvVar() {
    if (current_var.empty()) {
        output += "<>";
        return;
    }
    char* env_var_val{};
    size_t size{};
    if (_dupenv_s(&env_var_val, &size, current_var.c_str()) != 0 || !env_var_val) {
        output += '<' + current_var + '>';
        return;
    }
    output += env_var_val;
    free(env_var_val);
}

void EnvVarStrResolver::setInput(const std::string& input) {
    this->input = input;
}

const std::string& EnvVarStrResolver::getOutput() const {
    return output;
}
