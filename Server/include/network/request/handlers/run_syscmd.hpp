#pragma once

#include <string>

#include "../handler.hpp"
#include "Shared/user/role.hpp"
#include "Shared/util/version.hpp"

class RunSyscmdRH final : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
        .min_role = Role::Developer,
    };

    std::string command{}, output{};

    void _run() override;

    bool extractCommand();
    bool runCommand();
    void insertOutput();

  public:
    RunSyscmdRH();
};
