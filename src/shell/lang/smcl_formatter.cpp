#include "shell/lang/smcl_formatter.hpp"

#include <string>

SMCLFormatter::SMCLFormatter(const std::string& str) : input{str} {
}

void SMCLFormatter::processChar() {
    const char c{input[curr_idx]};
    if (isArgHere() && arg_idx < args_str.size()) {
        output.insert(output.end(), args_str[arg_idx].begin(), args_str[arg_idx].end());
        arg_idx++;
        curr_idx += ARG_LENGTH - 1;
    } else {
        output += c;
    }
}

bool SMCLFormatter::isArgHere() const {
    if (curr_idx + ARG_LENGTH > input.size()) {
        return false;
    }
    return input[curr_idx] == '$' && input[curr_idx + 1] == '{' && input[curr_idx + 2] == '}';
}

const std::string& SMCLFormatter::getOutput() const {
    return output;
}
