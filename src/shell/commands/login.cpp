#include "shell/commands/login.hpp"

#include <boost/asio/ip/tcp.hpp>

#include "Shared/io/console.hpp"
#include "Shared/user/role.hpp"
#include "network/client.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"

using boost::asio::ip::tcp;

exit_code_t LoginCommand::run() {
    if (args.size() < 2) {
        return InvalidArgs;
    }
    if (!getArgumentsValues()) {
        return Error;
    }
    return (login()) ? Success : Error;
}

bool LoginCommand::login() {
    if (!getConnectedUser()) {
        return false;
    }
    client->setUser(*connected_user);
    client->setLogged(true);
    console::out::verbose("-> ", false);
    console::out::inf(
        lang::gt("command.login.successfull_connect", connected_user->name, role::getName(connected_user->role)));
    return true;
}

bool LoginCommand::getConnectedUser() {
    handler.setClient(client);
    handler.setUsername(username);
    handler.setPassword(password);
    if (!handler.run()) {
        return false;
    }
    connected_user = &handler.getConnectedUser();
    return true;
}

bool LoginCommand::getArgumentsValues() {
    username = args[0];
    if (args.size() >= 2) {
        password = args[1];
    }
    return checkUsername() && checkPassword();
}

bool LoginCommand::checkUsername() const {
    if (username.empty()) {
        console::out::err(lang::gt("command.login.error.empty_username"));
        return false;
    }
    if (username.size() > 0x10) {
        console::out::err(lang::gt("command.login.error.long_username"));
        return false;
    }
    return true;
}

bool LoginCommand::checkPassword() const {
    if (password.size() > 0x10) {
        console::out::err(lang::gt("command.login.error.long_password"));
        return false;
    }
    return true;
}
