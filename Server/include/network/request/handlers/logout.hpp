#pragma once

#include "../handler.hpp"
#include "Shared/util/version.hpp"

class LogoutRH : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
    };

    void _run() override;

  public:
    LogoutRH();
};
