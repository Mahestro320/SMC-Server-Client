#include "network/request/handlers/io_move_file.hpp"

#include "io/sfs.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOMoveFileRH::IOMoveFileRH() : RH{REQUIREMENTS} {
}

void IOMoveFileRH::_run() {
    if (!request.extractPath(input_path) || !request.extractPath(output_path)) {
        return;
    }
    const SFS& sfs{client->getSFS()};
    if (!sfs.move(input_path, output_path, &response)) {
        return;
    }
    sendResponse();
}
