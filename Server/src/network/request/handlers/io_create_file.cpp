#include "network/request/handlers/io_create_file.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

#include "Shared/io/console.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/util/error_message.hpp"
#include "io/sfs.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOCreateFileRH::IOCreateFileRH() : RH{REQUIREMENTS} {
}

void IOCreateFileRH::_run() {
    extractPath() && checkPath() && createFile();
}

bool IOCreateFileRH::extractPath() {
    return request.extractAbsPath(*client, path, response);
}

bool IOCreateFileRH::checkPath() {
    const SFS& sfs{client->getSFS()};
    return sfs.checkPathAlreadyExist(path, &response);
}

bool IOCreateFileRH::createFile() {
    std::error_code ec{};
    fs::create_directories(path.parent_path(), ec);
    (void)ec;

    std::ofstream file{path};
    if (!file.is_open()) {
        response.buildError(ResponseErrorInfos{
            .error_id = ResponseError::IOCreateFileError,
            .precision_msg = "error while creating file \"" + path.string() + "\": " + util::error_message::get(errno),
        });
        return false;
    }
    return true;
}
