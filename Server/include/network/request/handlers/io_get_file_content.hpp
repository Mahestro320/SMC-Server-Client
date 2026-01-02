#pragma once

#include <filesystem>
#include <fstream>

#include "../handler.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/version.hpp"

class IOGetFileContentRH final : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
    };

    std::ifstream file{};
    std::filesystem::path target_path{};

    pds::network_buffer_t buffer_size{};
    pds::network_buffer_count_t buffer_count{};
    pds::network_buffer_count_t curr_idx{};
    pds::file_size_t file_size{};

    void _run() override;

    bool extractPath();
    bool extractBufferSize();
    bool checkBufferSize();
    bool openFile();
    void insertFileSize();
    void insertBufferCount();
    bool transmitData();

    void closeFile();

  public:
    IOGetFileContentRH();
    ~IOGetFileContentRH();
};
