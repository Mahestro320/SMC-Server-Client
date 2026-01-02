#include "shell/commands/infos.hpp"

#include "constants.hpp"
#include "Shared/io/console.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"

exit_code_t InfosCommand::run() {
    console::out::inf(lang::gt("global.begin_message", constants::VERSION.toString()));
    return Success;
}
