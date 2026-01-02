#pragma once

#include <filesystem>
#include <string>

#include "../handler.hpp"
#include "Shared/util/version.hpp"

class IOSetFileNameRH : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
    };

    std::filesystem::path path{};
    std::string new_name{};

    void _run() override;

    bool extractPath();
    bool extractNewName();
    bool checkPath();
    bool renameFile();

  public:
    IOSetFileNameRH();
};
