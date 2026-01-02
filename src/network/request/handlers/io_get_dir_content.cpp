#include "network/request/handlers/io_get_dir_content.hpp"

#include <filesystem>
#include <utility>
#include <vector>

#include "Shared/io/file_info.hpp"
#include "Shared/network/request/id.hpp"
#include "Shared/pds.hpp"
#include "network/request/handler.hpp"

IOGetDirContentRH::IOGetDirContentRH() : RH{RequestId::IOGetDirContent} {
}

bool IOGetDirContentRH::_run() {
    request.insertPath(path);
    if (!sendRequest() || !checkResponse()) {
        return false;
    }
    std::vector<char> bfl{};
    if (!response.extractBuffer<pds::bfl_t>(bfl)) {
        return false;
    }
    decoder.setBFL(std::move(bfl));
    if (!decoder.decode()) {
        return false;
    }
    data = decoder.getData();
    return true;
}

const std::vector<FileInfo>& IOGetDirContentRH::getData() const {
    return data;
}

void IOGetDirContentRH::setSilent(bool silent) {
    decoder.setSilent(silent);
}

void IOGetDirContentRH::setPath(const std::filesystem::path& path) {
    this->path = path;
}
