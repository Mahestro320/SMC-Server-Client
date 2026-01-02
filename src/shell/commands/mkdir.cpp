#include "shell/commands/mkdir.hpp"

#include "network/client.hpp"
#include "network/request/handlers/io_create_directory.hpp"
#include "shell/exit_code.hpp"
#include "users/user.hpp"

exit_code_t MkDirCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    const User& user{client->getUser()};
    IOCreateDirectoryRH handler{};
    handler.setClient(client);
    handler.setDirectoryPath(user.current_dir / args[0]);
    return (handler.run()) ? Success : Error;
}
