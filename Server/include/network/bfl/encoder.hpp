#pragma once

#include <vector>

#include "Shared/io/file_info.hpp"
#include "Shared/pds.hpp"

class BFLEncoder final {
  private:
    std::vector<FileInfo> data{};
    std::vector<char> bfl{};

    pds::file_size_t current_offset{};

    void pushFileCount();
    void allocMemoryForFiles();
    void pushFile(const FileInfo& file);
    void pushFileTypeOf(const FileInfo& file);
    void pushFileSizeOf(const FileInfo& file);
    void pushFileNameSizeOf(const FileInfo& file);
    void pushFileNameOf(const FileInfo& file);
    pds::file_size_t getFileSizeInBuffer(const FileInfo& file);

  public:
    BFLEncoder() = default;

    void setData(std::vector<FileInfo>&& data);
    void encode();
    const std::vector<char>& getBFL() const;
};
