#pragma once

#include "../handler.hpp"

#include <filesystem>
#include <ostream>

#include "Shared/pds.hpp"

class IOGetFileContentRH final : public RH {
  private:
    std::ostream* stream{};
    std::filesystem::path path{};
    pds::file_size_t size{};
    pds::network_buffer_count_t curr_idx{};
    pds::network_buffer_count_t buffer_count{};
    pds::network_buffer_t buffer_size{};

    bool _run() override;

    bool extractFileSize();
    bool extractBufferCount();
    bool checkStream() const;

  public:
    IOGetFileContentRH();

    void setStream(std::ostream* stream);
    void setPath(const std::filesystem::path& path);

    [[nodiscard]] bool isAvailable() const;
    [[nodiscard]] pds::network_buffer_count_t getBufferCount() const;
    [[nodiscard]] pds::network_buffer_count_t getCurrentBufferIndex() const;
    [[nodiscard]] pds::network_buffer_t getBufferSize();
    [[nodiscard]] pds::file_size_t getFileSize();

    bool downloadNextBuffer();
};
