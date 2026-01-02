#pragma once

#include <filesystem>

struct LangData;

class SMCLLoader final {
  private:
    LangData* data{};

  public:
    void setData(LangData* data);
    bool load(const std::filesystem::path& path);
};
