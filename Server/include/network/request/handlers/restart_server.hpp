#pragma once

#include "../handler.hpp"
#include "Shared/server/restart_modes.hpp"
#include "Shared/user/role.hpp"
#include "Shared/util/version.hpp"

class RestartServerRH final : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
        .min_role = Role::Developer,
    };

    RestartMode mode{};
    void (RestartServerRH::*restart_func_ptr)(void){};

    void _run() override;

    bool extractRestartMode();

    bool getRestartFuncPtr();

    void restartMachine();
    void restartProgram();

    bool enableShutdownPrivilege();

  public:
    RestartServerRH();
};
