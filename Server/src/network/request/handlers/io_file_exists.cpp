#include "network/request/handlers/io_file_exists.hpp"

#include "io/sfs.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"

IOFileExistsRH::IOFileExistsRH() : RH{REQUIREMENTS} {
}

void IOFileExistsRH::_run() {
    if (extractPath()) {
        response.insertBool(checkIfExists());
    }
}

bool IOFileExistsRH::extractPath() {
    return request.extractAbsPath(*client, path, response);
}

bool IOFileExistsRH::checkIfExists() const {
    const SFS& sfs{client->getSFS()};
    return sfs.exists(path);
}
