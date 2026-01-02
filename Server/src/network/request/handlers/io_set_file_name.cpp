#pragma once

#include "network/request/handlers/io_set_file_name.hpp"

#include <string>

#include "Shared/io/console.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/pds.hpp"
#include "io/sfs.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOSetFileNameRH::IOSetFileNameRH() : RH{REQUIREMENTS} {
}

void IOSetFileNameRH::_run() {
    extractPath() && extractNewName() && checkPath() && renameFile();
}

bool IOSetFileNameRH::extractPath() {
    return request.extractAbsPath(*client, path, response);
}

bool IOSetFileNameRH::extractNewName() {
    return request.extractString<pds::file_name_t>(new_name);
}

bool IOSetFileNameRH::checkPath() {
    const SFS& sfs{client->getSFS()};
    if (!sfs.checkPathNotExist(path, &response)) {
        response.buildError(ResponseErrorInfos{
            .error_id = ResponseError::IOFileNotExists,
            .precision_msg = "the file \"" + path.string() + "\" does not exists",
        });
        return false;
    }
    return true;
}

bool IOSetFileNameRH::renameFile() {
    console::out::inf("renaming file \"" + path.generic_string() + "\" to \"" + new_name + "\"");
    const SFS& sfs{client->getSFS()};
    return sfs.rename(path, new_name, &response);
}
