#include "network/request/handlers/io_get_file_type.hpp"

#include "Shared/pds.hpp"
#include "io/sfs.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"

IOGetFileTypeRH::IOGetFileTypeRH() : RH{REQUIREMENTS} {
}

void IOGetFileTypeRH::_run() {
    if (!extractPath()) {
        return;
    }
    getFileType();
    insertFileType();
}

bool IOGetFileTypeRH::extractPath() {
    return request.extractAbsPath(*client, path, response);
}

void IOGetFileTypeRH::getFileType() {
    const SFS& sfs{client->getSFS()};
    type = sfs.getFileType(path);
}

void IOGetFileTypeRH::insertFileType() {
    response.insertInt<pds::file_type_t>(static_cast<pds::file_type_t>(type));
}
