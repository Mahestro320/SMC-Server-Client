#pragma once

#include "../command.hpp"
#include "../exit_code.hpp"

class MoveCommand final : public Command {
  public:
    MoveCommand() = default;

    exit_code_t run() override;
};
