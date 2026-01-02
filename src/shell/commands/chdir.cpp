#include "shell/commands/chdir.hpp"

#include <cctype>
#include <filesystem>
#include <string>

#include "Shared/io/console.hpp"
#include "Shared/io/file_type.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_file_type.hpp"
#include "network/request/handlers/io_get_real_path.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"
#include "users/user.hpp"

namespace fs = std::filesystem;

exit_code_t ChDirCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    if (!getInputPath() || !isInputPathDirectory()) {
        return Error;
    }
    setNewCurrentDir();
    return Success;
}

bool ChDirCommand::getInputPath() {
    IOGetRealPathRH handler{};
    handler.setClient(client);
    const User& user{client->getUser()};
    handler.setPath(user.current_dir / fs::path{args[0]});
    if (!handler.run()) {
        return false;
    }
    target_path = handler.getValue();
    return true;
}

bool ChDirCommand::isInputPathDirectory() const {
    IOGetFileTypeRH handler{};
    handler.setClient(client);
    handler.setPath(target_path);
    if (!handler.run()) {
        return false;
    }
    if (handler.getValue() != FileType::Directory) {
        console::out::err(lang::gt("io.error.path_not_directory"));
        return false;
    }
    return true;
}

void ChDirCommand::setNewCurrentDir() {
    User& user{client->getUser()};
    user.current_dir = target_path.lexically_normal();
    const std::string current_dir_str{user.current_dir.generic_string()};
    if (current_dir_str.size() == 2 && std::isalpha(current_dir_str[0]) && current_dir_str[1] == ':') {
        user.current_dir = fs::path{current_dir_str + "\\"};
        user.current_dir = user.current_dir.lexically_normal();
    }
}
