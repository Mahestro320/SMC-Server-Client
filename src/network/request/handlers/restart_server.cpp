#include "network/request/handlers/restart_server.hpp"

#include "Shared/network/request/id.hpp"
#include "Shared/pds.hpp"
#include "Shared/server/restart_modes.hpp"
#include "network/request/handler.hpp"

RestartServerRH::RestartServerRH() : RH{RequestId::ServerRestart} {
}

bool RestartServerRH::_run() {
    request.insertInt<pds::restart_mode_t>(static_cast<pds::restart_mode_t>(restart_mode));
    return sendRequest() && checkResponse();
}

void RestartServerRH::setRestartMode(RestartMode mode) {
    restart_mode = mode;
}
