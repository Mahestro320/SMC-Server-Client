#pragma once

#include <filesystem>

#include "../handler.hpp"

class IOFileExistsRH final : public RH {
  private:
    std::filesystem::path path{};
    bool does_file_exists{};

    bool _run() override;

    bool processResponse();

  public:
    IOFileExistsRH();

    void setPath(const std::filesystem::path& path);
    bool getValue() const;
};
