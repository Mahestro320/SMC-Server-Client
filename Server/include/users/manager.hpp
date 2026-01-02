#pragma once

#include <string>
#include <vector>

#include "user.hpp"

class UsersManager final {
  private:
    std::vector<User> users{};

    UsersManager() = default;

    bool checkUsersFolderExistence() const;

  public:
    static UsersManager& getInstance();

    void load();
    bool exists(const std::string& username);
    User get(const std::string& username);
    bool set(const User& user);
};
