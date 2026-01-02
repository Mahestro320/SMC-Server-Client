#include "network/request/handlers/io_create_directory.hpp"

#include <filesystem>
#include <string>
#include <system_error>

#include "Shared/io/console.hpp"
#include "Shared/network/response/error.hpp"
#include "io/sfs.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOCreateDirectoryRH::IOCreateDirectoryRH() : RH{REQUIREMENTS} {
}

void IOCreateDirectoryRH::_run() {
    extractPath() && checkPath() && createDirectory();
}

bool IOCreateDirectoryRH::extractPath() {
    return request.extractAbsPath(*client, path, response);
}

bool IOCreateDirectoryRH::checkPath() {
    const SFS& sfs{client->getSFS()};
    return sfs.checkPathAlreadyExist(path, &response);
}

bool IOCreateDirectoryRH::createDirectory() {
    std::error_code ec{};
    if (!fs::create_directories(path, ec)) {
        response.buildError(ResponseErrorInfos{
            .error_id = ResponseError::IOCreateDirError,
            .precision_msg = "error while creating directory \"" + path.string() + "\": " + ec.message(),
        });
        return false;
    }
    return true;
}
