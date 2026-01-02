#include "shell/commands/user.hpp"

#include "shell/exit_code.hpp"

exit_code_t UserCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    return Success;
}
