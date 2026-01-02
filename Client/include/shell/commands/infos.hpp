#pragma once

#include "../command.hpp"
#include "../exit_code.hpp"

class InfosCommand : public Command {
  public:
    InfosCommand() = default;

    exit_code_t run() override;
};
