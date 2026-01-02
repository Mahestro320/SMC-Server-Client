#include "shell/commands/print.hpp"

#include <string>

#include "Shared/io/console.hpp"
#include "shell/exit_code.hpp"

exit_code_t PrintCommand::run() {
    std::string message{};
    for (size_t i{}; i < args.size(); i++) {
        message += ((i == 0) ? "" : "\t") + args[i];
    }
    console::out::inf(message);
    return Silent;
}
