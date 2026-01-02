#pragma once

#include <filesystem>

#include "../handler.hpp"
#include "Shared/util/version.hpp"

class IOMoveFileRH final : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
    };

    std::filesystem::path input_path{};
    std::filesystem::path output_path{};

    void _run() override;

  public:
    IOMoveFileRH();
};
