#pragma once

#include <filesystem>
#include <string>

#include "../handler.hpp"

class IOSetFileNameRH : public RH {
  private:
    std::filesystem::path path{};
    std::string new_name{};

    bool _run() override;

  public:
    IOSetFileNameRH();

    void setFilePath(const std::filesystem::path& path);
    void setNewName(const std::string& name);
};
