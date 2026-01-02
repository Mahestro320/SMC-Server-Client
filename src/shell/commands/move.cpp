#include "shell/commands/move.hpp"

#include "network/client.hpp"
#include "network/request/handlers/io_move_file.hpp"
#include "shell/exit_code.hpp"
#include "users/user.hpp"

exit_code_t MoveCommand::run() {
    if (args.size() < 2) {
        return InvalidArgs;
    }
    const User& user{client->getUser()};
    IOMoveFileRH handler{};
    handler.setClient(client);
    handler.setInputPath(user.current_dir / args[0]);
    handler.setOutputPath(user.current_dir / args[1]);
    return handler.run() ? Success : Error;
}
