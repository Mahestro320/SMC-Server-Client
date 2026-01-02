#include "network/request/handlers/io_get_real_path.hpp"

#include <filesystem>
#include <string>

#include "Shared/io/console.hpp"
#include "Shared/network/response/error.hpp"
#include "io/sfs.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOGetRealPathRH::IOGetRealPathRH() : RH{REQUIREMENTS} {
}

void IOGetRealPathRH::_run() {
    if (extractInputPath() && checkInputPath()) {
        getRealPath();
        insertRealPath();
    }
}

bool IOGetRealPathRH::extractInputPath() {
    return request.extractAbsPath(*client, input_path, response);
}

bool IOGetRealPathRH::checkInputPath() {
    const SFS& sfs{client->getSFS()};
    if (!sfs.exists(input_path)) {
        response.buildError(ResponseErrorInfos{
            .error_id = ResponseError::IOFileNotExists,
            .precision_msg = "the path \"" + input_path.string() + "\" does not exists",
        });
        return false;
    }
    return true;
}

void IOGetRealPathRH::getRealPath() {
    const SFS& sfs{client->getSFS()};
    real_path = sfs.real(input_path);
}

void IOGetRealPathRH::insertRealPath() {
    const SFS& sfs{client->getSFS()};
    const fs::path relative_real_path{sfs.relativeToUserScope(real_path)};
    response.insertPath(relative_real_path);
}
