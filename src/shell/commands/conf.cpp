#include "shell/commands/conf.hpp"

#include <string>

#include "Shared/io/console.hpp"
#include "shell/config.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"
#include "util/string.hpp"

exit_code_t ConfCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    std::string option{args[0]};
    util::string::toLowercase(option);
    return (runOption(option)) ? Success : Error;
}

bool ConfCommand::runOption(const std::string& option) {
    if (option == OPTION_LOAD) {
        return runLoadOption();
    } else if (option == OPTION_SAVE) {
        return runSaveOption();
    } else if (option == OPTION_PRINT) {
        return runPrintOption();
    } else if (option == OPTION_GET) {
        return runGetOption();
    } else if (option == OPTION_SET) {
        return runSetOption();
    }
    console::out::err(lang::gt("command._global.unknown_option", option));
    return false;
}

bool ConfCommand::runLoadOption() const {
    Config& config{Config::getInstance()};
    return config.load();
}

bool ConfCommand::runSetOption() const {
    if (args.size() < 3) {
        console::out::err(lang::gt("command._global.error.invalid_arg"));
        return false;
    }
    Config& config{Config::getInstance()};
    return config.setValue(args[1], args[2]);
}

bool ConfCommand::runSaveOption() const {
    Config& config{Config::getInstance()};
    return config.updateFile();
}

bool ConfCommand::runGetOption() const {
    if (args.size() < 2) {
        console::out::err(lang::gt("command._global.invalid_arg"));
        return false;
    }
    const std::string& key{args[1]};
    Config& config{Config::getInstance()};
    const std::string value{config.getValue(key)}, resolved_value{config.getResolvedValue(key)};
    console::out::inf(lang::gt("command.conf.key_value", key, value));
    if (value != resolved_value) {
        console::out::inf(lang::gt("command.conf.key_resolved_value", key, resolved_value));
    }
    return true;
}

bool ConfCommand::runPrintOption() const {
    const Config& config{Config::getInstance()};
    bool first{true};
    for (const auto& section : config.getPropertyTree()) {
        if (!first) {
            console::out::inf();
        }
        first = false;
        console::out::inf(lang::gt("command.conf.section_name", section.first));
        for (const auto& kv : section.second) {
            console::out::inf(lang::gt("command.conf.binding", kv.first, kv.second.get_value<std::string>()));
        }
    }
    return true;
}
