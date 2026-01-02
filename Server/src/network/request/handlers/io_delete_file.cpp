#include "network/request/handlers/io_delete_file.hpp"

#include <filesystem>
#include <string>
#include <system_error>

#include "Shared/io/console.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/pds.hpp"
#include "io/sfs.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IODeleteFileRH::IODeleteFileRH() : RH{REQUIREMENTS} {
}

void IODeleteFileRH::_run() {
    if (extractPath() && checkPath() && deleteFile()) {
        response.insertInt<pds::file_count_t>(elems_removed);
    }
}

bool IODeleteFileRH::extractPath() {
    return request.extractAbsPath(*client, path, response);
}

bool IODeleteFileRH::checkPath() {
    const SFS& sfs{client->getSFS()};
    return sfs.checkPathNotExist(path, &response);
}

bool IODeleteFileRH::deleteFile() {
    std::error_code ec{};
    elems_removed = static_cast<pds::file_count_t>(fs::remove_all(path, ec));
    if (ec) {
        response.buildError(ResponseErrorInfos{
            .error_id = ResponseError::IODeleteError,
            .precision_msg = "error while removing file \"" + path.string() + "\": " + ec.message(),
        });
        return false;
    }
    console::out::inf("successfully removed " + std::to_string(elems_removed) + " elems");
    return true;
}
