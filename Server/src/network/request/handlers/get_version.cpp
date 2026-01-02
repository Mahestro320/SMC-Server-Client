#include "network/request/handlers/get_version.hpp"

#include "Shared/pds.hpp"
#include "common.hpp"
#include "network/request/handler.hpp"

GetVersionRH::GetVersionRH() : RH{REQUIREMENTS} {
}

void GetVersionRH::_run() {
    response.insertInt<pds::version_t>(common::VERSION.combine());
}
