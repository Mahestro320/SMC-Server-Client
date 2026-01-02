#pragma once

#include <filesystem>

#include "../handler.hpp"

class IOCreateDirectoryRH final : public RH {
  private:
    std::filesystem::path path{};
    
    bool _run() override;

  public:
    IOCreateDirectoryRH();

    void setDirectoryPath(const std::filesystem::path& path);
};
