#pragma once

#include <filesystem>
#include <string>

#include "Shared/pds.hpp"
#include "Shared/user/role.hpp"

struct User final {
    std::string name{}, password{};
    Role role{Role::None};
    pds::wrong_pw_count_t wrong_password_count{};
    std::filesystem::path perso_directory{};

    bool is_valid{true};
};
