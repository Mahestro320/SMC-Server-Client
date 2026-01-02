#pragma once

#include "../command.hpp"
#include "../exit_code.hpp"

class QuitCommand final : public Command {
  public:
    QuitCommand() = default;

    exit_code_t run() override;
};
