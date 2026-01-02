#pragma once

#include "../command.hpp"
#include "../exit_code.hpp"

class DeleteCommand final : public Command {
  public:
    DeleteCommand() = default;

    exit_code_t run() override;
};
