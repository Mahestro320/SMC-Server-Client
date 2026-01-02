#include "network/request/handlers/io_move_file.hpp"

#include <filesystem>

#include "Shared/network/request/id.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOMoveFileRH::IOMoveFileRH() : RH{RequestId::IOMoveFile} {
}

void IOMoveFileRH::setInputPath(const fs::path& path) {
    this->input_path = path;
}

void IOMoveFileRH::setOutputPath(const fs::path& path) {
    this->output_path = path;
}

bool IOMoveFileRH::_run() {
    request.insertPath(input_path);
    request.insertPath(output_path);
    return sendRequest() && checkResponse();
}
