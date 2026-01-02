#include "network/request/handlers/io_set_file_name.hpp"

#include <filesystem>
#include <string>

#include "Shared/network/request/id.hpp"
#include "Shared/pds.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOSetFileNameRH::IOSetFileNameRH() : RH{RequestId::IOSetFileName} {
}

bool IOSetFileNameRH::_run() {
    request.insertPath(path);
    request.insertString<pds::file_name_t>(new_name);
    return sendRequest() && checkResponse();
}

void IOSetFileNameRH::setFilePath(const std::filesystem::path& path) {
    this->path = path;
}

void IOSetFileNameRH::setNewName(const std::string& name) {
    new_name = name;
}
