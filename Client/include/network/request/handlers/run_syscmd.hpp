#pragma once

#include <string>

#include "../handler.hpp"

class RunSyscmdRH final : public RH {
  private:
    std::string command{}, output{};

    bool _run() override;

    bool extractOutput();

  public:
    RunSyscmdRH();

    void setCommand(const std::string& command);
    const std::string& getOutput() const;
};
