#include "network/request/handlers/io_create_file.hpp"

#include <filesystem>

#include "Shared/network/request/id.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOCreateFileRH::IOCreateFileRH() : RH{RequestId::IOCreateFile} {
}

bool IOCreateFileRH::_run() {
    request.insertPath(path);
    return sendRequest() && checkResponse();
}

void IOCreateFileRH::setFilePath(const fs::path& path) {
    this->path = path;
}
