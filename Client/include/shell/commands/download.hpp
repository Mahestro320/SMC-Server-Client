#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

#include "../command.hpp"
#include "../exit_code.hpp"

class DownloadCommand final : public Command {
  private:
    std::vector<std::filesystem::path> input_files{};
    std::filesystem::path input_path{};
    std::ofstream curr_output_stream{};

    void closeFile();

    void getInputPath();
    bool getInputFiles();
    bool getInputFilesFromFolder();

    bool initStreamFor(const std::filesystem::path& path);
    bool getOutputPathFor(const std::filesystem::path& path, std::filesystem::path& output);
    bool downloadFile(const std::filesystem::path& path);

  public:
    DownloadCommand() = default;
    ~DownloadCommand();

    exit_code_t run() override;
};
