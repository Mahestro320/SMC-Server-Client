#pragma once

#include <cstdint>

using exit_code_t = uint16_t;

enum ExitCode : exit_code_t {
    Success,
    Error,
    InvalidArgs,
    Silent,
};
