#include "Shared/user/role.hpp"

#include <string>
#include <unordered_map>

namespace {

static const std::string UNKNOWN{"???"};

const std::unordered_map<Role, std::string> role_names{
    {Role::None, "NONE"},
    {Role::User, "USER"},
    {Role::Admin, "ADMIN"},
    {Role::Developer, "DEVELOPER"},
};

} // namespace

const std::string& role::getName(Role role) {
    return (isValidRole(role)) ? role_names.at(role) : UNKNOWN;
}

bool role::isValidRole(Role role) {
    return role_names.contains(role);
}
