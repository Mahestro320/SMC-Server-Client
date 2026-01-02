#include "network/request/handlers/io_get_file_type.hpp"

#include <filesystem>

#include "Shared/io/console.hpp"
#include "Shared/io/file_type.hpp"
#include "Shared/network/request/id.hpp"
#include "Shared/pds.hpp"
#include "network/request/handler.hpp"
#include "shell/lang.hpp"

namespace fs = std::filesystem;

IOGetFileTypeRH::IOGetFileTypeRH() : RH{RequestId::IOGetFileType} {
}

bool IOGetFileTypeRH::_run() {
    request.insertPath(path);
    return sendRequest() && checkResponse() && extractFileType();
}

bool IOGetFileTypeRH::extractFileType() {
    pds::file_type_t byte{};
    if (!response.extractInt<pds::file_type_t>(byte)) {
        return false;
    }
    const FileType file_type{static_cast<FileType>(byte)};
    if (!file_type::isValid(file_type)) {
        console::out::verbose(lang::gt("rh.io_get_file_type.error.invalid_file_type", byte));
        return false;
    }
    this->file_type = file_type;
    return true;
}

void IOGetFileTypeRH::setPath(const fs::path& path) {
    this->path = path;
}

FileType IOGetFileTypeRH::getValue() const {
    return file_type;
}
