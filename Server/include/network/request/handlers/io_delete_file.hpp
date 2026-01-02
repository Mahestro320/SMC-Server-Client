#pragma once

#include <filesystem>

#include "../handler.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/version.hpp"

class IODeleteFileRH final : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
    };

    std::filesystem::path path{};
    pds::file_count_t elems_removed{};

    void _run() override;

    bool extractPath();
    bool checkPath();
    bool deleteFile();

  public:
    IODeleteFileRH();
};
