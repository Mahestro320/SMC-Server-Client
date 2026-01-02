#include "network/request/handlers/logout.hpp"

#include "network/client.hpp"
#include "network/request/handler.hpp"

LogoutRH::LogoutRH() : RH{REQUIREMENTS} {
}

void LogoutRH::_run() {
    client->setLogged(false);
    client->setUser({});
}
