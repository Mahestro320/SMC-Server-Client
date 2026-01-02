#include "network/request/handlers/logout.hpp"

#include "Shared/network/request/id.hpp"
#include "network/request/handler.hpp"

LogoutRH::LogoutRH() : RH{RequestId::Logout} {
}

bool LogoutRH::_run() {
    return sendRequest() && checkResponse();
}
