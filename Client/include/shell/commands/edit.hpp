#pragma once

#include "../command.hpp"
#include "../exit_code.hpp"

class EditCommand final : public Command {
  public:
    EditCommand() = default;

    exit_code_t run() override;
};
