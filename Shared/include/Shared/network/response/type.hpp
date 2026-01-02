#pragma once

#include "Shared/pds.hpp"

enum class ResponseType : pds::response_type_t {
    Ok = 0x00,
    Error = 0xFF,
};
