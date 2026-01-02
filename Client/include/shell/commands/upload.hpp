#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include "../command.hpp"
#include "../exit_code.hpp"
#include "Shared/pds.hpp"

class UploadCommand final : public Command {
  private:
    std::filesystem::path input_path{};
    std::filesystem::path output_folder{};
    std::vector<std::filesystem::path> input_files{};
    std::ifstream curr_input_file{};
    pds::file_size_t curr_file_size{};

    void closeFile();

    void getPathsFromArgs();
    bool getInputFiles();
    bool openFile(const std::filesystem::path& path);
    bool uploadFile(const std::filesystem::path& path);
    bool getOutputPathFor(const std::filesystem::path& path, std::filesystem::path& output_path);

  public:
    UploadCommand() = default;
    ~UploadCommand();

    exit_code_t run() override;
};
