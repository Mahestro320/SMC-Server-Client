#include "network/request/handlers/login.hpp"

#include <chrono>
#include <string>
#include <thread>

#include "Shared/io/console.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/pds.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"
#include "users/manager.hpp"

LoginRH::LoginRH() : RH{REQUIREMENTS} {
}

void LoginRH::_run() {
    extractUsername() && extractPassword() && getUserFromUsername() && checkPassword();
    insertUserRole();
    client->setUser(user);
    client->setLogged(true);
}

bool LoginRH::extractUsername() {
    if (!request.extractString<pds::username_t>(username)) {
        response.buildErrorFromInvalidRequest(request);
        return false;
    }
    return true;
}

bool LoginRH::extractPassword() {
    if (!request.extractString<pds::password_t>(password)) {
        response.buildErrorFromInvalidRequest(request);
        return false;
    }
    return true;
}

bool LoginRH::getUserFromUsername() {
    console::out::inf("getting user");
    UsersManager& users_manager{UsersManager::getInstance()};
    user = users_manager.get(username);
    if (!user.is_valid) {
        response.buildError(ResponseErrorInfos{.error_id = ResponseError::UserNotExists});
        return false;
    }
    return true;
}

bool LoginRH::checkPassword() {
    console::out::inf("checking password");
    UsersManager& users_manager{UsersManager::getInstance()};
    if (password == user.password) {
        user.wrong_password_count = 0;
        return users_manager.set(user);
    }
    console::out::warn("invalid password !");
    const pds::timestamp_t wait_time_in_seconds{static_cast<pds::timestamp_t>(user.wrong_password_count) *
                                                static_cast<pds::timestamp_t>(user.wrong_password_count)};
    if (wait_time_in_seconds != 0) {
        console::out::inf("waiting " + std::to_string(wait_time_in_seconds) + " seconds");
        std::this_thread::sleep_for(std::chrono::seconds{wait_time_in_seconds});
    }
    user.wrong_password_count++;
    users_manager.set(user);
    response.buildError(ResponseErrorInfos{.error_id = ResponseError::InvalidPassword});
    return false;
}

void LoginRH::insertUserRole() {
    response.insertInt<pds::role_t>(static_cast<pds::role_t>(user.role));
}
