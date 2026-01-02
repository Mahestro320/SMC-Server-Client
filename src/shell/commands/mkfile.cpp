#include "shell/commands/mkfile.hpp"

#include "network/client.hpp"
#include "network/request/handlers/io_create_file.hpp"
#include "shell/exit_code.hpp"
#include "users/user.hpp"

exit_code_t MkFileCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    const User& user{client->getUser()};
    IOCreateFileRH handler{};
    handler.setClient(client);
    handler.setFilePath(user.current_dir / args[0]);
    return (handler.run()) ? Success : Error;
}
