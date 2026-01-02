#pragma once

#include <filesystem>

#include "../handler.hpp"
#include "Shared/pds.hpp"

class IOGetFileSizeRH final : public RH {
  private:
    pds::file_size_t size{};
    std::filesystem::path path{};

    bool _run() override;

    bool extractFileSize();

  public:
    IOGetFileSizeRH();

    void setPath(const std::filesystem::path& path);
    pds::file_size_t getValue() const;
};
