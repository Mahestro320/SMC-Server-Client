#pragma once

#include "../command.hpp"
#include "../exit_code.hpp"

class PrintCommand final : public Command {
  public:
    PrintCommand() = default;

    exit_code_t run() override;
};
