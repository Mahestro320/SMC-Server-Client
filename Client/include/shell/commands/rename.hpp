#pragma once

#include <filesystem>

#include "../command.hpp"
#include "../exit_code.hpp"

class RenameCommand final : public Command {
  private:
    std::filesystem::path raw_path{}, complete_path{};

  public:
    RenameCommand() = default;

    exit_code_t run() override;
};
