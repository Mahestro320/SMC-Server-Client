#include "network/request/handlers/io_delete_file.hpp"

#include <filesystem>

#include "Shared/io/console.hpp"
#include "Shared/network/request/id.hpp"
#include "Shared/pds.hpp"
#include "network/request/handler.hpp"
#include "shell/lang.hpp"

namespace fs = std::filesystem;

IODeleteFileRH::IODeleteFileRH() : RH{RequestId::IODeleteFile} {
}

bool IODeleteFileRH::_run() {
    request.insertPath(path);
    if (!sendRequest() || !checkResponse() || !response.extractInt<pds::file_count_t>(elems_removed)) {
        return false;
    }
    console::out::inf(lang::gt("rh.io_delete_file.successfull_delete", elems_removed));
    return true;
}

void IODeleteFileRH::setFilePath(const std::filesystem::path& path) {
    this->path = path;
}

pds::file_count_t IODeleteFileRH::getElemsRemoved() const {
    return elems_removed;
}
