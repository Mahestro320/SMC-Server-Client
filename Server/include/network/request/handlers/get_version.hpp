#pragma once

#include "../handler.hpp"
#include "Shared/util/version.hpp"

class GetVersionRH final : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
    };

    void _run() override;

  public:
    GetVersionRH();
};
