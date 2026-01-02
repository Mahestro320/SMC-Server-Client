#pragma once

#include <string>
#include <vector>

#include "exit_code.hpp"

class Client;

class Command {
  protected:
    Client* client{};
    std::vector<std::string> args{};

  public:
    Command() = default;

    void setClient(Client* client);
    void setArgs(const std::vector<std::string>& args);

    virtual exit_code_t run() = 0;
};
