#pragma once

#include <istream>
#include <string>

#include "Shared/pds.hpp"
#include "file_loader.hpp"
#include "network/request/handlers/io_set_file_content.hpp"

class FileUploader final : public FileLoader {
  private:
    std::istream* stream{};
    pds::file_size_t size{};
    IOSetFileContentRH handler{};

    void initHandler();
    void initInfoPrinter();
    std::string getFileSizeAsStr();

  public:
    FileUploader() = default;

    void setStream(std::istream* stream);
    void setStreamSize(pds::file_size_t size);
    bool uploadFile();
};
