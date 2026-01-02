#include "network/request/handlers/io_file_exists.hpp"

#include <filesystem>

#include "Shared/network/request/id.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOFileExistsRH::IOFileExistsRH() : RH{RequestId::IOFileExists} {
}

bool IOFileExistsRH::_run() {
    request.insertPath(path);
    return sendRequest() && checkResponse() && processResponse();
}

bool IOFileExistsRH::processResponse() {
    return response.extractBool(does_file_exists);
}

void IOFileExistsRH::setPath(const fs::path& path) {
    this->path = path;
}

bool IOFileExistsRH::getValue() const {
    return does_file_exists;
}
