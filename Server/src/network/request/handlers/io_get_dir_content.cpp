#include "network/request/handlers/io_get_dir_content.hpp"

#include <exception>
#include <filesystem>
#include <string>
#include <utility>

#include "Shared/io/console.hpp"
#include "Shared/io/file_info.hpp"
#include "Shared/io/file_type.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/pds.hpp"
#include "io/sfs.hpp"
#include "network/bfl/encoder.hpp"
#include "network/request/handler.hpp"

namespace fs = std::filesystem;

IOGetDirContentRH::IOGetDirContentRH() : RH{REQUIREMENTS} {
}

void IOGetDirContentRH::_run() {
    if (!extractPath() || !checkPath()) {
        return;
    }
    try {
        getFileList();
    } catch (const std::exception& e) {
        console::out::err(e);
    }
    insertBuffer();
}

bool IOGetDirContentRH::extractPath() {
    console::out::inf("getting the path to list");
    return request.extractAbsPath(*client, path, response);
}

bool IOGetDirContentRH::checkPath() {
    const SFS& sfs{client->getSFS()};
    if (!sfs.checkPathNotExist(path, &response)) {
        return false;
    }
    if (!sfs.isDirectory(path)) {
        response.buildError(ResponseErrorInfos{
            .error_id = ResponseError::IOPathNotDirectory,
            .precision_msg = "the path \"" + path.string() + "\" isn't a directory",
        });
        return false;
    }
    return true;
}

void IOGetDirContentRH::getFileList() {
    const SFS& sfs{client->getSFS()};
    for (const fs::path& file : sfs.getDirectoryFileList(path)) {
        try {
            pushFileInList(file);
        } catch (const std::exception& e) {
            console::out::err(e);
        }
    }
}

void IOGetDirContentRH::pushFileInList(const fs::path& file) {
    const std::filesystem::path& name_path{file.filename()};
    FileInfo file_info{
        .name = name_path.string(),
        .type = FileType::None,
    };
    if (fs::is_regular_file(file)) {
        file_info.type = FileType::File;
        file_info.size = static_cast<pds::file_size_t>(fs::file_size(file));
    } else if (fs::is_directory(file)) {
        file_info.type = FileType::Directory;
    }
    data.push_back(file_info);
}

void IOGetDirContentRH::insertBuffer() {
    BFLEncoder encoder{};
    encoder.setData(std::move(data));
    encoder.encode();
    response.insertBuffer<pds::bfl_t>(encoder.getBFL());
}
