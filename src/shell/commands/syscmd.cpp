#include "shell/commands/syscmd.hpp"

#include <string>

#include "Shared/io/console.hpp"
#include "network/request/handlers/run_syscmd.hpp"
#include "shell/exit_code.hpp"

exit_code_t SyscmdCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    RunSyscmdRH handler{};
    handler.setClient(client);
    handler.setCommand(args[0]);
    if (!handler.run()) {
        return Error;
    }
    const std::string& output{handler.getOutput()};
    console::out::inf("output:" + (output.empty() ? " (empty)" : "\n" + output), false);
    return Success;
}
