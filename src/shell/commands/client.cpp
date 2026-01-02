#include "shell/commands/client.hpp"

#include <string>

#include "Shared/io/console.hpp"
#include "network/client.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"
#include "util/string.hpp"

exit_code_t ClientCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    std::string option{args[0]};
    util::string::toLowercase(option);
    return (runOption(option)) ? Success : Error;
}

bool ClientCommand::runOption(const std::string& option) {
    if (option == OPTION_CONNECT) {
        return runConnectOption();
    } else if (option == OPTION_DISCONNECT) {
        return runDisconnectOption();
    }
    console::out::err(lang::gt("command._global.error.unknown_option", option));
    return false;
}

bool ClientCommand::runConnectOption() {
    return client->connect();
}

bool ClientCommand::runDisconnectOption() {
    client->close();
    return true;
}
