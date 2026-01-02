#pragma once

#include <string>
#include <vector>

#include "Shared/io/file_info.hpp"
#include "Shared/io/file_type.hpp"
#include "Shared/pds.hpp"

// BFL = Buffered File List
class BFLDecoder {
  private:
    std::vector<char> bfl{};
    std::vector<FileInfo> data{};

    bool is_silent{};
    pds::file_count_t file_count{};
    pds::file_size_t current_offset{};

    void printVerbose(const std::string& message, bool end_line = true);

    bool getFileCount();
    bool decodeFile();
    bool getFileType(FileType& type);
    bool getFileSize(pds::file_size_t& size);
    bool getFileName(std::string& name);

  public:
    BFLDecoder() = default;

    void setSilent(bool silent);
    void setBFL(std::vector<char>&& bfl);

    bool decode();
    const std::vector<FileInfo>& getData() const;
};
