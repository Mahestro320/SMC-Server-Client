#include "shell/commands/server.hpp"

#include <string>

#include "Shared/io/console.hpp"
#include "Shared/server/restart_modes.hpp"
#include "Shared/user/role.hpp"
#include "Shared/util/version.hpp"
#include "network/client.hpp"
#include "network/request/handlers/get_version.hpp"
#include "network/request/handlers/restart_server.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"
#include "users/user.hpp"
#include "util/string.hpp"

exit_code_t ServerCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    std::string option{args[0]};
    util::string::toLowercase(option);
    return (runOption(option)) ? Success : Error;
}

bool ServerCommand::runOption(const std::string& option) {
    if (option == OPTION_GET_VERSION) {
        return runGetVersionOption();
    } else if (option == OPTION_RESTART) {
        return runRestartSubOption();
    }
    console::out::err(lang::gt("command._global.error.unknown_option", option));
    return false;
}

bool ServerCommand::runGetVersionOption() {
    GetVersionRH handler{};
    handler.setClient(client);
    if (!handler.run()) {
        return false;
    }
    const Version& version{handler.getValue()};
    console::out::inf(lang::gt("command.server.version", version.toString(), version.combine()));
    return true;
}

bool ServerCommand::runRestartSubOption() {
    if (!canRestart()) {
        return false;
    }
    std::string option{OPTION_RESTART_PROGRAM};
    if (args.size() >= 2 && !args[1].empty()) {
        option = args[1];
    }
    RestartMode restart_mode{RestartMode::Program};
    if (option == OPTION_RESTART_PROGRAM) {
        restart_mode = RestartMode::Program;
    } else if (option == OPTION_RESTART_MACHINE) {
        restart_mode = RestartMode::Program;
    } else {
        console::out::err(lang::gt("command._global.error.unknown_sub_option", option));
        return false;
    }
    if (!restartServer(restart_mode)) {
        return false;
    }
    client->setLogged(false);
    client->setUser(User{});
    client->close();
    return true;
}

bool ServerCommand::restartServer(RestartMode mode) {
    RestartServerRH handler{};
    handler.setClient(client);
    handler.setRestartMode(mode);
    return handler.run();
}

bool ServerCommand::canRestart() const {
    if (!client->isLogged()) {
        console::out::err(lang::gt("command._global.error.cannot_run_cmd.not_logged"));
        return false;
    }
    const User& user{client->getUser()};
    if (user.role < Role::Developer) {
        console::out::err(lang::gt("command._global.error.cannot_run_cmd.invalid_perms"));
        return false;
    }
    return true;
}
