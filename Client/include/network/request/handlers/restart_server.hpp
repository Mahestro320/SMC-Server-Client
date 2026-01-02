#pragma once

#include "../handler.hpp"
#include "Shared/server/restart_modes.hpp"

class RestartServerRH final : public RH {
  private:
    RestartMode restart_mode{};

    bool _run() override;

  public:
    RestartServerRH();

    void setRestartMode(RestartMode mode);
};
