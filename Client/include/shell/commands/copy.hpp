#pragma once

#include "../command.hpp"
#include "../exit_code.hpp"

class CopyCommand final : public Command {
  public:
    CopyCommand() = default;

    exit_code_t run() override;
};
