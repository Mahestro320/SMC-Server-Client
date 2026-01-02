#include "network/request/handlers/login.hpp"

#include <filesystem>
#include <string>

#include "Shared/network/request/id.hpp"
#include "Shared/pds.hpp"
#include "Shared/user/role.hpp"
#include "network/request/handler.hpp"
#include "users/user.hpp"

namespace fs = std::filesystem;

LoginRH::LoginRH() : RH{RequestId::Login} {
}

bool LoginRH::_run() {
    insertUserInfos();
    if (!sendRequest() || !checkResponse() || !extractUserRole()) {
        return false;
    }
    buildUser();
    return true;
}

void LoginRH::insertUserInfos() {
    request.insertString<pds::username_t>(user.name);
    request.insertString<pds::password_t>(user.password);
}

bool LoginRH::extractUserRole() {
    pds::role_t byte{};
    if (!response.extractInt<pds::role_t>(byte)) {
        return false;
    }
    user.role = static_cast<Role>(byte);
    return true;
}

void LoginRH::buildUser() {
#pragma warning(push)
#pragma warning(disable : 26813)
    if (user.role == Role::User) {
        user.current_dir = fs::path{"perso." + user.name};
    } else if (user.role == Role::Admin) {
        user.current_dir = fs::path{"network"} / ("perso." + user.name);
    } else if (user.role == Role::Developer) {
        user.current_dir = fs::path{"C:\\"};
    }
#pragma warning(pop)
}

void LoginRH::setUsername(const std::string& username) {
    user.name = username;
}

void LoginRH::setPassword(const std::string& password) {
    user.password = password;
}

const User& LoginRH::getConnectedUser() const {
    return user;
}
