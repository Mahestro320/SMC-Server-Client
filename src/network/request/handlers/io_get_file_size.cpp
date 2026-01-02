#include "network/request/handlers/io_get_file_size.hpp"

#include <filesystem>

#include "Shared/io/console.hpp"
#include "Shared/network/request/id.hpp"
#include "Shared/pds.hpp"
#include "network/request/handler.hpp"
#include "shell/lang.hpp"

namespace fs = std::filesystem;

IOGetFileSizeRH::IOGetFileSizeRH() : RH{RequestId::IOGetFileSize} {
}

bool IOGetFileSizeRH::_run() {
    request.insertPath(path);
    return sendRequest() && checkResponse() && extractFileSize();
}

bool IOGetFileSizeRH::extractFileSize() {
    if (!response.extractInt<pds::file_size_t>(size)) {
        return false;
    }
    console::out::verbose(lang::gt("rh.io_get_file_size.file_size", size));
    return true;
}

void IOGetFileSizeRH::setPath(const fs::path& path) {
    this->path = path;
}

pds::file_size_t IOGetFileSizeRH::getValue() const {
    return size;
}
