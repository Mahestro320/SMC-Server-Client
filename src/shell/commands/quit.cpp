#include "shell/commands/quit.hpp"

#include "common.hpp"
#include "shell/exit_code.hpp"

exit_code_t QuitCommand::run() {
    common::is_running = false;
    return Success;
}
