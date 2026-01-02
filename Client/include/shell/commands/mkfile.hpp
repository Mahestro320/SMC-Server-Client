#pragma once

#include "../command.hpp"
#include "../exit_code.hpp"

class MkFileCommand final : public Command {
  public:
    MkFileCommand() = default;

    exit_code_t run() override;
};
