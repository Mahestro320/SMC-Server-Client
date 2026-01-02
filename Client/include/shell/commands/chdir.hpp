#pragma once

#include <filesystem>

#include "../command.hpp"
#include "../exit_code.hpp"

class ChDirCommand final : public Command {
  private:
    std::filesystem::path target_path{};

    bool getInputPath();
    bool isInputPathDirectory() const;
    void setNewCurrentDir();

  public:
    ChDirCommand() = default;

    exit_code_t run() override;
};
