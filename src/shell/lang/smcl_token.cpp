#include "shell/lang/smcl_token.hpp"

#include <string>
#include <variant>

std::string Token::contentToString() const {
    if (std::holds_alternative<std::string>(content)) {
        return get<std::string>(content);
    } else if (std::holds_alternative<int>(content)) {
        return std::to_string(get<int>(content));
    }
    return "???";
}
