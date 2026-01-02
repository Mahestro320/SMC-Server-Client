#pragma once

#include <filesystem>

#include "../handler.hpp"
#include "Shared/util/version.hpp"

class IOCreateDirectoryRH final : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
    };

    std::filesystem::path path{};

    void _run() override;

    bool extractPath();
    bool checkPath();
    bool createDirectory();

  public:
    IOCreateDirectoryRH();
};
