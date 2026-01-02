#include "shell/commands/lang.hpp"

#include <filesystem>
#include <string>
#include <system_error>

#include "Shared/io/console.hpp"
#include "Shared/util/version.hpp"
#include "io/directories.hpp"
#include "shell/config.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"

namespace fs = std::filesystem;

exit_code_t LangCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    const std::string option{args[0]};
    return runOption(option) ? Success : Error;
}

bool LangCommand::runOption(const std::string& option) {
    if (option == OPTION_INFO) {
        return runOptionInfo();
    } else if (option == OPTION_RELOAD) {
        return runOptionReload();
    } else if (option == OPTION_SET) {
        return runOptionSet();
    }
    console::out::err(lang::gt("command._global.error.unknown_option", option));
    return false;
}

bool LangCommand::runOptionInfo() {
    console::out::inf(lang::gt("command.lang.infos") + '\n');
    console::out::inf(lang::gt("command.lang.name", lang::getName()));
    console::out::inf(lang::gt("command.lang.locale", lang::getLocale()));
    const Version& version{lang::getVersion()};
    console::out::inf(lang::gt("command.lang.version", version.toString()));
    console::out::inf(lang::gt("command.lang.authors"));
    for (const std::string& author : lang::getAuthors()) {
        console::out::inf(" - " + author);
    }
    return true;
}

bool LangCommand::runOptionReload() {
    return lang::loadFile();
}

bool LangCommand::runOptionSet() {
    if (args.size() < 2) {
        console::out::inf(lang::gt("command._global.invalid_args"));
        return false;
    }
    const std::string arg{args[1]};
    if (tryToSetWithFullFileName(arg) || tryToSetWithFileName(arg)) {
        return lang::loadFile();
    }
    console::out::err(lang::gt("command.lang.unknown_lang", arg));
    return false;
}

bool LangCommand::tryToSetWithFullFileName(const std::string& arg) {
    std::error_code ec{};
    const fs::path target{dirs::D_LANG / arg};
    if (!fs::exists(target) || ec) {
        return false;
    }
    Config& config{Config::getInstance()};
    ConfigValues& config_values{config.getModifiableValues()};
    config_values.shell_language = target.stem().string();
    return true;
}

bool LangCommand::tryToSetWithFileName(const std::string& arg) {
    std::error_code ec{};
    const fs::path target{dirs::D_LANG / (arg + '.' + SMCL_EXTENSION)};
    if (!fs::exists(target) || ec) {
        return false;
    }
    Config& config{Config::getInstance()};
    ConfigValues& config_values{config.getModifiableValues()};
    config_values.shell_language = arg;
    return true;
}
