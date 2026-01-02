#pragma once

#include "../handler.hpp"

class LogoutRH final : public RH {
  private:
    bool _run() override;

  public:
    LogoutRH();
};
