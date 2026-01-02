#pragma once

#include <filesystem>
#include <vector>

#include "../handler.hpp"
#include "Shared/io/file_info.hpp"
#include "Shared/util/version.hpp"

class IOGetDirContentRH : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
    };

    std::vector<FileInfo> data{};
    std::filesystem::path path{};

    void _run() override;

    bool extractPath();
    bool checkPath();
    void getFileList();
    void pushFileInList(const std::filesystem::path& path);
    void insertBuffer();

  public:
    IOGetDirContentRH();
};
