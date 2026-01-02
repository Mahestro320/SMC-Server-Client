#include "network/request/handlers/io_create_directory.hpp"

#include <filesystem>

#include "Shared/network/request/id.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOCreateDirectoryRH::IOCreateDirectoryRH() : RH{RequestId::IOCreateDirectory} {
}

bool IOCreateDirectoryRH::_run() {
    request.insertPath(path);
    return sendRequest() && checkResponse();
}

void IOCreateDirectoryRH::setDirectoryPath(const fs::path& path) {
    this->path = path;
}
