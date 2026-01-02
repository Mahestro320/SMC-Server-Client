#include "users/loader.hpp"

#include <filesystem>
#include <string>

#include "Shared/io/console.hpp"
#include "Shared/user/role.hpp"
#include "io/directories.hpp"
#include "users/fs_config.hpp"
#include "users/uif/decoder.hpp"
#include "users/uif/values.hpp"
#include "users/user.hpp"

namespace fs = std::filesystem;

UserLoader::UserLoader(const fs::path& folder_path) : folder_path{folder_path} {
}

bool UserLoader::load() {
    const fs::path rel_folder_path{fs::relative(folder_path, dirs::strg)};
    UIFDecoder uif_decoder{};
    if (!uif_decoder.loadDataFromPath(folder_path / UIF_FILE_NAME) || !uif_decoder.decode()) {
        console::out::err("error while loading user \"" + rel_folder_path.string() + "\"");
        return false;
    }
    loadUserFromUIFValues(uif_decoder.getValues());
    user.perso_directory = dirs::s_network / (std::string{user_fs_config::PERSO_DIR_PREFIX} + "." + user.name);
    if (!createPersoDirectory()) {
        return false;
    }
    console::out::inf("loaded user <:color=bright_blue>" + user.name + "<:color=reset> (<:color=green>" +
                      role::getName(user.role) + "<:color=reset>)");
    return true;
}

void UserLoader::loadUserFromUIFValues(const UIFValues& values) {
    user.name = values.username;
    user.password = values.password;
    user.role = static_cast<Role>(values.role);
}

bool UserLoader::createPersoDirectory() const {
    if (!fs::exists(user.perso_directory) && !fs::create_directories(user.perso_directory)) {
        console::out::err("error while creating perso dir for user " + user.name);
        return false;
    }
    return true;
}

User UserLoader::getUser() const {
    return user;
}
