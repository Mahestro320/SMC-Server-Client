#include "users/manager.hpp"

#include <filesystem>
#include <string>

#include "Shared/io/console.hpp"
#include "Shared/pds.hpp"
#include "io/directories.hpp"
#include "users/loader.hpp"
#include "users/user.hpp"

namespace fs = std::filesystem;

UsersManager& UsersManager::getInstance() {
    static UsersManager instance{};
    return instance;
}

void UsersManager::load() {
    console::out::inf("loading users");
    if (!checkUsersFolderExistence()) {
        return;
    }
    pds::user_count_t users_count{};
    pds::user_count_t users_loaded_count{};
    for (const fs::path& path : fs::directory_iterator{dirs::s_users}) {
        ++users_count;
        UserLoader loader{path};
        if (!loader.load()) {
            continue;
        }
        ++users_loaded_count;
        users.push_back(loader.getUser());
    }
    if (users_count == 0) {
        console::out::inf("no user found");
        return;
    }
    console::out::inf(std::to_string(users_loaded_count) + "/" + std::to_string(users_count) + " users loaded");
}

bool UsersManager::checkUsersFolderExistence() const {
    if (fs::exists(dirs::s_users)) {
        return true;
    }
    console::out::err("the users directory does not exists");
    fs::create_directories(dirs::s_users);
    return false;
}

bool UsersManager::exists(const std::string& username) {
    const User user{get(username)};
    return user.is_valid;
}

User UsersManager::get(const std::string& username) {
    for (const User& user : users) {
        if (user.name == username) {
            return user;
        }
    }
    return User{.is_valid = false};
}

bool UsersManager::set(const User& user) {
    for (User& curr_user : users) {
        if (user.name == curr_user.name) {
            curr_user = user;
            return true;
        }
    }
    return false;
}
