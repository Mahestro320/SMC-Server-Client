#include "Shared/util/error_message.hpp"

#include <string.h>
#include <string>

std::string util::error_message::get(int errnum) {
    char buffer[0xFF];
    if (strerror_s(buffer, sizeof(buffer), errnum) == 0) {
        return std::string{buffer};
    }
    return "Unknown error";
}
