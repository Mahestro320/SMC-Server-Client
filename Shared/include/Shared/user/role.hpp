#pragma once

#include <string>

#include "Shared/pds.hpp"

enum class Role : pds::role_t {
    User,
    Admin,
    Developer,
    None = 0xFF,
};

namespace role {

extern const std::string& getName(Role role);
extern bool isValidRole(Role role);

} // namespace role
