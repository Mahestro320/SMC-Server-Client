#pragma once

#include "../command.hpp"
#include "../exit_code.hpp"

class MkDirCommand final : public Command {
  public:
    MkDirCommand() = default;

    exit_code_t run() override;
};
