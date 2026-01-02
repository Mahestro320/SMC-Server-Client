#include "shell/commands/logout.hpp"

#include "network/client.hpp"
#include "network/request/handlers/logout.hpp"
#include "shell/exit_code.hpp"
#include "users/user.hpp"

exit_code_t LogoutCommand::run() {
    LogoutRH handler{};
    handler.setClient(client);
    if (!handler.run()) {
        return Error;
    }
    client->setLogged(false);
    client->setUser(User{});
    return Success;
}
