#include "shell/commands/rename.hpp"

#include "network/client.hpp"
#include "network/request/handlers/io_set_file_name.hpp"
#include "shell/exit_code.hpp"
#include "users/user.hpp"

exit_code_t RenameCommand::run() {
    if (args.size() < 2) {
        return InvalidArgs;
    }
    const User& user{client->getUser()};
    IOSetFileNameRH handler{};
    handler.setClient(client);
    handler.setFilePath(user.current_dir / args[0]);
    handler.setNewName(args[1]);
    return handler.run() ? Success : Error;
}
