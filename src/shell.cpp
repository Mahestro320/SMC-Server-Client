#include "shell.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <cstdint>
#include <string>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/util/time.hpp"
#include "constants.hpp"
#include "network/client.hpp"
#include "shell/command.hpp"
#include "shell/command_line.hpp"
#include "shell/commands_infos.hpp"
#include "shell/config.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"
#include "shell/system/signal.hpp"
#include "users/user.hpp"
#include "util/string.hpp"

using boost::asio::ip::tcp;

Shell::Shell(Client& client) : client{client} {
}

void Shell::start() {
    shell::signal::setSignals(this);
    console::out::inf(lang::gt("global.begin_message", constants::VERSION.toString()) + ", " +
                      lang::gt("shell.begin_message_ext"));
}

void Shell::processNewCommand() {
    console::out::inf(getCommandInputStartInfo() + "> ", false);
    const std::vector<std::string> command_tokens{getCommandTokens()};
    if (command_tokens.empty()) {
        return;
    }
    const exit_code_t code{runCommand(command_tokens)};
    printFinalCmdMessage(code);
    console::out::inf();
    return;
}

std::vector<std::string> Shell::getCommandTokens() {
    CommandLine command_line{};
    if (!command_line.getLine() || command_line.empty()) {
        return {};
    }
    command_line.tokenize();
    return command_line.getTokens();
}

exit_code_t Shell::runCommand(const std::vector<std::string>& command_tokens) {
    std::string name{command_tokens[0]};
    util::string::toLowercase(name);
    const std::vector<std::string> args{getArgsFromTokens(command_tokens)};
    std::unique_ptr<Command> command{getCommandInstanceFromName(name)};
    if (!command.get()) {
        console::out::err(lang::gt("command._global.error.unknown_cmd", name));
        return Silent;
    }
    if (!canExecuteCmdName(name)) {
        return Error;
    }
    command->setArgs(args);
    command->setClient(&client);
    const uint64_t start_time{util::time::getMillis()};
    const exit_code_t code{command->run()};
    command_time = static_cast<float>(util::time::getMillis() - start_time) / 1000.0f;
    return code;
}

bool Shell::canExecuteCmdName(const std::string& name) const {
    if (commands_infos.contains(name)) {
        return canExecuteCmdInstance(commands_infos.at(name));
    }
    for (const auto& [_, cmd] : commands_infos) {
        if (cmd.short_name == name) {
            return canExecuteCmdInstance(cmd);
        }
    }
    console::out::err(lang::gt("command._global.error.no_cmd_found_for", name));
    return true;
}

bool Shell::canExecuteCmdInstance(const CommandInfos& cmd) const {
    const User& user{client.getUser()};
    if (!cmd.must_be_logged) {
        return true;
    }
    if (!client.isLogged()) {
        console::out::err(lang::gt("command._global.error.cannot_run_cmd.not_logged"));
        return false;
    } else if (user.role < cmd.min_role) {
        console::out::err(lang::gt("command._global.error.cannot_run_cmd.invalid_perms"));
        return false;
    }
    return true;
}

std::unique_ptr<Command> Shell::getCommandInstanceFromName(const std::string& name) const {
    for (const auto& [cmd_name, cmd_infos] : commands_infos) {
        if (name == cmd_name || name == cmd_infos.short_name) {
            return (*commands_infos[cmd_name].cmd)();
        }
    }
    return nullptr;
}

Client& Shell::getClient() {
    return client;
}

std::vector<std::string> Shell::getArgsFromTokens(std::vector<std::string> tokens) const {
    tokens.erase(tokens.begin());
    return tokens;
}

std::string Shell::getCommandInputStartInfo() const {
    if (!client.isConnected()) {
        return "NOT_CONNECTED";
    }
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    const std::string addr_prefix{config_values.server_address + ":" + std::to_string(config_values.server_port) +
                                  " -> "};
    if (!client.isLogged()) {
        return ((config_values.shell_print_addr_prefix) ? addr_prefix : "") + "NOT_LOGGED";
    }
    const User& user{client.getUser()};
    return ((config_values.shell_print_addr_prefix) ? addr_prefix : "") + user.name + " @" +
           ((user.current_dir.empty()) ? "." : user.current_dir.string());
}

void Shell::printFinalCmdMessage(exit_code_t code) const {
    if (code == ExitCode::Silent) {
        return;
    }
    if (code == ExitCode::InvalidArgs) {
        console::out::err(lang::gt("command._global.error.invalid_args"));
        return;
    }
    const std::string time_str{((command_time < 1.0f) ? std::to_string(static_cast<int>(command_time * 1000.0f)) + "ms"
                                                      : util::string::formatFloat(command_time, 3) + 's')};
    console::out::inf('\n' + lang::gt("shell.command_finished", time_str, code) +
                      (code == ExitCode::Error ? lang::gt("shell.command_is_err") : ""));
}
