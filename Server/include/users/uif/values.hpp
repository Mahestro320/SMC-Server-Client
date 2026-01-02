#pragma once

#include <string>

#include "Shared/pds.hpp"

struct UIFValues {
    pds::role_t role{};
    std::string username{}, password{};
};
