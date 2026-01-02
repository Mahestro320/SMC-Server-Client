#pragma once

#include <filesystem>
#include <vector>

#include "../handler.hpp"
#include "Shared/io/file_info.hpp"
#include "network/bfl/decoder.hpp"

class IOGetDirContentRH final : public RH {
  private:
    std::vector<FileInfo> data{};
    std::filesystem::path path{};
    BFLDecoder decoder{};

    bool _run() override;

  public:
    IOGetDirContentRH();

    const std::vector<FileInfo>& getData() const;

    void setSilent(bool is_silent);
    void setPath(const std::filesystem::path& path);
};
