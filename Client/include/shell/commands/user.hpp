#pragma once

#include "../command.hpp"
#include "../exit_code.hpp"

class UserCommand final : public Command {
  public:
    UserCommand() = default;

    exit_code_t run() override;
};
