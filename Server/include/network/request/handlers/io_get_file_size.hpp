#pragma once

#include <filesystem>

#include "../handler.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/version.hpp"

class IOGetFileSizeRH final : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
    };

    std::filesystem::path path{};
    pds::file_size_t size{};

    void _run() override;

    bool extractTargetPath();
    void getFileSize();
    void insertFileSize();

  public:
    IOGetFileSizeRH();
};
