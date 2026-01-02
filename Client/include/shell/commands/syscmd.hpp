#pragma once

#include "../command.hpp"
#include "../exit_code.hpp"

class SyscmdCommand : public Command {
  public:
    exit_code_t run() override;
};
