#pragma once

#include <filesystem>

#include "../handler.hpp"
#include "Shared/io/file_type.hpp"

class IOGetFileTypeRH final : public RH {
  private:
    std::filesystem::path path{};
    FileType file_type{FileType::None};

    bool _run() override;

    bool extractFileType();

  public:
    IOGetFileTypeRH();

    void setPath(const std::filesystem::path& path);
    FileType getValue() const;
};
