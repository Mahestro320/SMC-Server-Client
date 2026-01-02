#include "network/request/handlers/util/get_dir_content_recursively.hpp"

#include <filesystem>
#include <utility>
#include <vector>

#include "Shared/io/file_info.hpp"
#include "Shared/io/file_type.hpp"
#include "network/request/handlers/io_get_dir_content.hpp"

namespace fs = std::filesystem;

bool GetDirContentRecursively::run() {
    return addContentOf(input_path);
}

bool GetDirContentRecursively::addContentOf(const fs::path& path) {
    IOGetDirContentRH handler{};
    handler.setClient(client);
    handler.setPath(path);
    handler.setSilent(true);
    if (!handler.run()) {
        console::out::inf(path.string());
        return false;
    }
    const std::vector<FileInfo>& file_infos{handler.getData()};
    content.reserve(content.size() + file_infos.size());
    for (const FileInfo& file_info : file_infos) {
        if (file_info.type == FileType::Directory && !addContentOf(path / file_info.name)) {
            return false;
        } else if (file_info.type == FileType::File) {
            content.push_back(path / file_info.name);
        }
    }
    return true;
}

void GetDirContentRecursively::setPath(const fs::path& path) {
    input_path = path;
}

const std::vector<fs::path>& GetDirContentRecursively::getContent() const {
    return content;
}

std::vector<fs::path>&& GetDirContentRecursively::getContent() {
    return std::move(content);
}
