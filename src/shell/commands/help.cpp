#include "shell/commands/help.hpp"

#include <string>

#include "Shared/io/console.hpp"
#include "Shared/user/role.hpp"
#include "network/client.hpp"
#include "shell/commands_infos.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"
#include "users/user.hpp"
#include "util/string.hpp"

using namespace util::string;

exit_code_t HelpCommand::run() {
    if (args.empty()) {
        printAllCommandsInfos();
    } else {
        return printLongDescription();
    }
    return Success;
}

bool HelpCommand::canSee(const CommandInfos& cmd) const {
    const User& user{client->getUser()};
    return user.role >= cmd.min_role;
}

void HelpCommand::printAllCommandsInfos() const {
    console::out::inf(lang::gt("command.help.infos"));
    printBeginMessage();
    const bool is_client_logged{client->isLogged()};
    for (const auto& [key, value] : commands_infos) {
        if ((value.must_be_logged && !is_client_logged) || !canSee(value)) {
            continue;
        }
        printCommandInfos(key, value);
    }
}

void HelpCommand::printBeginMessage() const {
    const std::string& name{lang::gt("command.help.name")};
    const std::string& short_name{lang::gt("command.help.short_name")};
    const std::string& short_description{lang::gt("command.help.short_description")};
    const std::string& can_use_offline{lang::gt("command.help.can_use_offline")};

    console::out::inf(VERTICAL_BAR + name + makeFilledString(NAME_SPACES_COUNT - name.size()) + VERTICAL_BAR +
                      short_name + makeFilledString(NAME_SPACES_COUNT - short_name.size()) + VERTICAL_BAR +
                      short_description + makeFilledString(SHORT_DESCR_SPACES_COUNT - short_description.size()) +
                      VERTICAL_BAR + can_use_offline +
                      makeFilledString(CAN_USE_OFFLINE_SPACES_COUNT - can_use_offline.size()) + '\n');
}

void HelpCommand::printCommandInfos(const std::string& cmd_name, const CommandInfos& cmd) const {
    bool short_descr_exists{};
    const std::string short_description_key{"command_info." + cmd_name + ".short_description"};
    const std::string short_description =
        ((lang::exists(short_description_key)) ? lang::gt(short_description_key) : "");
    const size_t vb_size{std::string{VERTICAL_BAR}.size()};
    console::out::inf(BEGIN_SPACES + cmd_name + makeFilledString(NAME_SPACES_COUNT - cmd_name.size() + vb_size) +
                      cmd.short_name + makeFilledString(SHORT_NAME_SPACES_COUNT - cmd.short_name.size() + vb_size) +
                      short_description +
                      makeFilledString(SHORT_DESCR_SPACES_COUNT - utf8Length(short_description) + vb_size) +
                      boolToYesOrNo(cmd.can_use_offline));
}

exit_code_t HelpCommand::printLongDescription() const {
    std::string target_cmd_name{args[0]};
    toLowercase(target_cmd_name);
    for (const auto& [cmd_name, cmd] : commands_infos) {
        if (target_cmd_name == cmd_name || target_cmd_name == cmd.short_name) {
            printLongDescriptionOf(cmd_name, cmd);
            return Success;
        }
    }
    console::out::err(lang::gt("command.help.error.unknown_cmd", args[0]));
    return Error;
}

void HelpCommand::printLongDescriptionOf(const std::string& cmd_name, const CommandInfos& cmd) const {
    const std::string& short_description{lang::gt("command_info." + cmd_name + ".short_description")};
    const std::string long_description_key{"command_info." + cmd_name + ".long_description"};
    if (canSee(cmd)) {
        console::out::inf((lang::exists(long_description_key)) ? lang::gt(long_description_key) : short_description);
        return;
    }
    const User& user{client->getUser()};
    console::out::err(lang::gt("command.help.error.cannot_use_cmd", role::getName(user.role)));
}
