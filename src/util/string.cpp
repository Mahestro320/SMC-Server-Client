#include "util/string.hpp"

#include <string>

#include "shell/lang.hpp"

std::string util::string::boolToYesOrNo(bool boolean) {
    return (boolean) ? lang::gt("global.yes") : lang::gt("global.no");
}
