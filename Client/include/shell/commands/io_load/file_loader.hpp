#pragma once

#include <filesystem>

#include "network/with_client.hpp"
#include "shell/commands/io_load/info_printer.hpp"

class FileLoader : public WithClient {
  protected:
    IOLoadInfoPrinter info_printer{};
    std::filesystem::path target_path{};

    FileLoader() = default;

  public:
    void setTargetPath(const std::filesystem::path& path);
};
