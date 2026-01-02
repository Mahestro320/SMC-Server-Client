#pragma once

#include <string>

#include "../command.hpp"
#include "../exit_code.hpp"
#include "network/request/handlers/login.hpp"
#include "user.hpp"

class LoginCommand final : public Command {
  private:
    LoginRH handler{};

    std::string username{}, password{};
    const User* connected_user{};

    bool getArgumentsValues();
    bool login();
    bool getConnectedUser();
    bool checkUsername() const;
    bool checkPassword() const;

  public:
    LoginCommand() = default;

    exit_code_t run() override;
};
