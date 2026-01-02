#include "network/request/handlers/io_get_file_size.hpp"

#include <string>

#include "Shared/io/console.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/pds.hpp"
#include "io/sfs.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOGetFileSizeRH::IOGetFileSizeRH() : RH{REQUIREMENTS} {
}

void IOGetFileSizeRH::_run() {
    if (!extractTargetPath()) {
        return;
    }
    getFileSize();
    insertFileSize();
}

bool IOGetFileSizeRH::extractTargetPath() {
    return request.extractAbsPath(*client, path, response);
}

void IOGetFileSizeRH::getFileSize() {
    const SFS& sfs{client->getSFS()};
    if (!sfs.exists(path)) {
        response.buildError(ResponseErrorInfos{
            .error_id = ResponseError::IOFileNotExists,
            .precision_msg = "the file \"" + path.string() + "\" does not exists",
        });
        return;
    }
    size = sfs.getFileSize(path);
    return;
}

void IOGetFileSizeRH::insertFileSize() {
    response.insertInt<pds::file_size_t>(size);
}
