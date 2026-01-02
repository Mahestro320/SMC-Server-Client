#pragma once

#include "../handler.hpp"
#include "Shared/util/version.hpp"

class GetVersionRH final : public RH {
  private:
    Version version{};

    bool _run() override;

    bool extractVersion();

  public:
    GetVersionRH();

    const Version& getValue() const;
};
