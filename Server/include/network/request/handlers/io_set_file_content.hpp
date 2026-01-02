#pragma once

#include <filesystem>
#include <fstream>

#include "../handler.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/version.hpp"

class IOSetFileContentRH final : public RH {
  private:
    static inline constexpr RHRequirements REQUIREMENTS{
        .min_version = Version{1, 0, 0},
        .must_be_logged = true,
    };

    std::ofstream file{};
    std::filesystem::path target_path{};

    pds::network_buffer_t buffer_size{};
    pds::network_buffer_count_t buffer_count{};
    pds::network_buffer_count_t curr_idx{};
    pds::file_size_t file_size{};

    void _run() override;

    bool extractPath();
    bool extractFileSize();
    bool extractBufferCount();
    bool extractBufferSize();
    bool checkBufferSize();
    bool openFile();
    bool transmitData();

    void closeFile();

  public:
    IOSetFileContentRH();
    ~IOSetFileContentRH();
};
