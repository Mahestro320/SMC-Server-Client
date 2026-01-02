#pragma once

#include <filesystem>

#include "../handler.hpp"
#include "Shared/io/file_type.hpp"
#include "Shared/util/version.hpp"

class IOGetFileTypeRH final : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
    };

    std::filesystem::path path{};
    FileType type{};

    void _run() override;

    bool extractPath();
    void getFileType();
    void insertFileType();

  public:
    IOGetFileTypeRH();
};
