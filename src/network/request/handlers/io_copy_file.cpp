#include "network/request/handlers/io_copy_file.hpp"

#include <filesystem>

#include "Shared/network/request/id.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOCopyFileRH::IOCopyFileRH() : RH{RequestId::IOCopyFile} {
}

void IOCopyFileRH::setInputPath(const fs::path& path) {
    this->input_path = path;
}

void IOCopyFileRH::setOutputPath(const fs::path& path) {
    this->output_path = path;
}

bool IOCopyFileRH::_run() {
    request.insertPath(input_path);
    request.insertPath(output_path);
    return sendRequest() && checkResponse();
}
