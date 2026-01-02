#pragma once

#include <filesystem>

#include "../handler.hpp"

class IOCreateFileRH : public RH {
  private:
    std::filesystem::path path{};

    bool _run() override;

  public:
    IOCreateFileRH();

    void setFilePath(const std::filesystem::path& path);
};
