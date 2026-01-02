#pragma once

#include <filesystem>

#include "../handler.hpp"

class IOMoveFileRH final : public RH {
  private:
    std::filesystem::path input_path{};
    std::filesystem::path output_path{};

    bool _run() override;

  public:
    IOMoveFileRH();

    void setInputPath(const std::filesystem::path& path);
    void setOutputPath(const std::filesystem::path& path);
};
