#pragma once

#include <string>

#include "../handler.hpp"
#include "users/user.hpp"

class LoginRH final : public RH {
  private:
    User user{};

    void insertUserInfos();
    bool extractUserRole();
    void buildUser();

    bool _run() override;

  public:
    LoginRH();

    void setUsername(const std::string& username);
    void setPassword(const std::string& password);

    const User& getConnectedUser() const;
};
