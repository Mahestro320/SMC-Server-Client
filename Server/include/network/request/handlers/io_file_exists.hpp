#pragma once

#include <filesystem>

#include "../handler.hpp"
#include "Shared/util/version.hpp"

class IOFileExistsRH final : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
    };

    void _run() override;

    std::filesystem::path path{};

    bool extractPath();
    bool checkIfExists() const;

  public:
    IOFileExistsRH();
};
