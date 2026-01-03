#include "network/request/handlers/io_copy_file.hpp"

#include "io/sfs.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOCopyFileRH::IOCopyFileRH() : RH{REQUIREMENTS} {
}

void IOCopyFileRH::_run() {
    if (!request.extractPath(input_path) || !request.extractPath(output_path)) {
        return;
    }
    const SFS& sfs{client->getSFS()};
    if (!sfs.copy(sfs.absoluteFromUserScope(input_path), sfs.absoluteFromUserScope(output_path), &response)) {
        return;
    }
    sendResponse();
}
