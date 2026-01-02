#include "network/request/handlers/io_get_real_path.hpp"

#include <filesystem>

#include "Shared/network/request/id.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOGetRealPathRH::IOGetRealPathRH() : RH{RequestId::IOGetRealPath} {
}

bool IOGetRealPathRH::_run() {
    request.insertPath(input_path);
    return sendRequest() && checkResponse() && extractRealPath();
}

bool IOGetRealPathRH::extractRealPath() {
    return response.extractPath(real_path);
}

void IOGetRealPathRH::setPath(const std::filesystem::path& input_path) {
    this->input_path = input_path;
}

const std::filesystem::path& IOGetRealPathRH::getValue() const {
    return real_path;
}
