#pragma once

#include <filesystem>

#include "../handler.hpp"

class IOGetRealPathRH final : public RH {
  private:
    std::filesystem::path input_path{}, real_path{};

    bool _run() override;

    bool extractRealPath();

  public:
    IOGetRealPathRH();

    void setPath(const std::filesystem::path& input_path);
    const std::filesystem::path& getValue() const;
};
