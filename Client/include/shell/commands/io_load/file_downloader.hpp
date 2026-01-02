#pragma once

#include <ostream>
#include <string>

#include "file_loader.hpp"
#include "network/request/handlers/io_get_file_content.hpp"

class FileDownloader final : public FileLoader {
  private:
    std::ostream* stream{};
    IOGetFileContentRH handler{};

    void initHandler();
    void initInfoPrinter();
    std::string getFileSizeAsStr();

  public:
    FileDownloader() = default;

    void setStream(std::ostream* stream);
    bool downloadFile();
};
