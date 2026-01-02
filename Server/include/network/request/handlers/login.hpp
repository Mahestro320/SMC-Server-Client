#pragma once

#include <string>

#include "../handler.hpp"
#include "Shared/util/version.hpp"
#include "users/user.hpp"

class LoginRH final : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
    };

    std::string username{}, password{};
    User user{};

    void _run() override;

    bool extractUsername();
    bool extractPassword();
    bool getUserFromUsername();
    bool checkPassword();
    void insertUserRole();

  public:
    LoginRH();
};
