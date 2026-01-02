#include "network/request/handlers/get_version.hpp"

#include "Shared/network/request/id.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/version.hpp"
#include "network/request/handler.hpp"

GetVersionRH::GetVersionRH() : RH{RequestId::GetVersion} {
}

bool GetVersionRH::_run() {
    return sendRequest() && checkResponse() && extractVersion();
}

bool GetVersionRH::extractVersion() {
    pds::version_t byte{};
    if (!response.extractInt<pds::version_t>(byte)) {
        return false;
    }
    version.setFromCombined(byte);
    return true;
}

const Version& GetVersionRH::getValue() const {
    return version;
}
