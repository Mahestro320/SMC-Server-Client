#include "io/sfs.hpp"

#include <algorithm>
#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/io/file_type.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/pds.hpp"
#include "Shared/user/role.hpp"
#include "Shared/util/string.hpp"
#include "io/directories.hpp"
#include "network/client.hpp"
#include "network/response/builder.hpp"
#include "users/user.hpp"

namespace fs = std::filesystem;

#pragma warning(push)
#pragma warning(disable : 26813)

SFS::SFS(Client& client) : client{client} {
}

fs::path SFS::absoluteFromUserScope(const fs::path& path) const {
    const User& user{client.getUser()};
    if (user.role == Role::User) {
        return dirs::s_network / path;
    } else if (user.role == Role::Admin) {
        return dirs::strg / path;
    }
    return path;
}

fs::path SFS::relativeToUserScope(const fs::path& path) const {
    const User& user{client.getUser()};
    if (user.role == Role::User) {
        return relative(path, dirs::s_network);
    } else if (user.role == Role::Admin) {
        return relative(path, dirs::strg);
    }
    return path;
}

fs::path SFS::relative(const fs::path& path, const fs::path& base) const {
    std::error_code ec{};
    const fs::path rel_path{fs::relative(path, base, ec)};
    if (ec) {
        console::out::err(ec.message());
        return path;
    }
    return rel_path;
}

fs::path SFS::real(const fs::path& path) const {
    std::error_code ec{};
    const fs::path real_path{fs::weakly_canonical(path.lexically_normal(), ec)};
    if (ec) {
        console::out::err(ec.message());
        return path;
    }
    return real_path;
}

fs::path SFS::removeEndBackslash(const fs::path& path) const {
    std::string path_str{path.string()};
    if (!path_str.empty() && (path_str.back() == '\\' || path_str.back() == '/')) {
        path_str.pop_back();
    }
    return fs::path{path_str};
}

std::vector<fs::path> SFS::getDirectoryFileList(const fs::path& path) const {
    if (!isDirectory(path)) {
        return {};
    }
    std::error_code ec{};
    std::vector<fs::path> result{};
    for (const fs::path& file : fs::directory_iterator{path, ec}) {
        if (canLexicallyAccess(file)) {
            result.push_back(file);
        }
    }
    if (ec) {
        console::out::err(ec.message());
    }
    return result;
}

FileType SFS::getFileType(const fs::path& path) const {
    if (isRegularFile(path)) {
        return FileType::File;
    } else if (isDirectory(path)) {
        return FileType::Directory;
    }
    return FileType::None;
}

pds::file_size_t SFS::getFileSize(const fs::path& path) const {
    std::error_code ec{};
    pds::file_size_t size{static_cast<pds::file_size_t>(fs::file_size(path, ec))};
    if (ec) {
        console::out::err(ec.message());
        return 0U;
    }
    return size;
}

bool SFS::isInDirectory(const fs::path& path, const fs::path& directory) const {
    const fs::path target_path{real(path)};
    const fs::path dir_path{real(directory)};

    auto path_it{target_path.begin()};
    for (auto dir_it{dir_path.begin()}; dir_it != dir_path.end(); ++dir_it, ++path_it) {
        if (path_it == target_path.end() || *path_it != *dir_it) {
            return false;
        }
    }
    return true;
}

bool SFS::canLexicallyAccess(fs::path path) const {
    path = path.lexically_normal();
    if (!client.isLogged()) {
        return false;
    }
    const User& user{client.getUser()};
    if (user.role == Role::Developer) {
        return true;
    } else if (user.role == Role::Admin) {
        return isInDirectory(path, dirs::strg) && !isUIFFile(path);
    } else if (user.role == Role::User) {
        return isSame(path, dirs::s_network) || (isInUPCDirectory(path) || isInDirectory(path, dirs::s_network_shared));
    }
    return true;
}

bool SFS::exists(const fs::path& path) const {
    std::error_code ec{};
    const bool does_exists{fs::exists(path, ec) && canLexicallyAccess(path)};
    if (ec) {
        console::out::err(ec.message());
    }
    return !ec && does_exists;
}

bool SFS::isSame(const fs::path& first, const fs::path& second) const {
    const fs::path first_normal{removeEndBackslash(first.lexically_normal())};
    const fs::path second_normal{removeEndBackslash(second.lexically_normal())};
    return first_normal.generic_string() == second_normal.generic_string();
}

bool SFS::isRegularFile(const fs::path& path) const {
    std::error_code ec{};
    const bool is_regular_file{exists(path) && fs::is_regular_file(path, ec)};
    if (ec) {
        console::out::err(ec.message());
    }
    return !ec && is_regular_file;
}

bool SFS::isDirectory(const fs::path& path) const {
    std::error_code ec{};
    const bool is_regular_file{exists(path) && fs::is_directory(path, ec)};
    if (ec) {
        console::out::err(ec.message());
    }
    return !ec && is_regular_file;
}

bool SFS::isInUPCDirectory(const fs::path& path) const {
    const User& user{client.getUser()};
    return isInDirectory(path, dirs::s_network / fs::path{"perso." + user.name});
}

bool SFS::isPUDDirectory(const fs::path& path) const {
    return isInDirectory(path, dirs::s_users) && path != dirs::s_users && isDirectory(path);
}

bool SFS::isUIFFile(const fs::path& path) const {
    return isPUDDirectory(path.parent_path()) && path.filename() == ".uif";
}

bool SFS::isValidName(const std::string& name) const {
    if (name.empty()) {
        return false;
    }
    if (std::any_of(name.begin(), name.end(),
                    [&](char c) { return INVALID_FILENAME_CHARS.find(c) != std::string::npos; }) ||
        name.back() == ' ' || name.back() == '.') {
        return false;
    }

    std::string lower_name{name};
    util::string::toLowercase(lower_name);
    for (const auto& reserved : RESERVED_FILE_NAMES) {
        if (lower_name == reserved || lower_name.rfind(reserved + ".", 0) == 0) {
            return false;
        }
    }
    return true;
}

bool SFS::rename(const fs::path& path, const std::string& new_name, ResponseBuilder* response) const {
    if (!isValidName(new_name)) {
        buildResponse(
            ResponseErrorInfos{
                .error_id = ResponseError::IOInvalidFileName,
                .precision_msg = "invalid file name: " + new_name,
            },
            response);
    }
    const fs::path parent_path{path.parent_path()};
    const fs::path target_path{parent_path / new_name};
    if (!canLexicallyAccess(path) || !canLexicallyAccess(parent_path) || !canLexicallyAccess(target_path)) {
        buildResponse(ResponseErrorInfos{.error_id = ResponseError::InvalidPermissions}, response);
        return false;
    }
    std::error_code ec{};
    fs::rename(path, target_path, ec);
    if (ec) {
        buildResponse(
            ResponseErrorInfos{
                .error_id = ResponseError::IORenameError,
                .precision_msg = "error while renaming file: " + ec.message(),
            },
            response);
        return false;
    }
    return true;
}

bool SFS::move(const fs::path& input_path, fs::path output_path, ResponseBuilder* response) const {
    output_path = output_path.lexically_normal();
    const fs::path target_path{output_path / input_path.filename()};
    if (!beginCopyOrMove(input_path, target_path, response)) {
        return false;
    }
    std::error_code ec{};
    fs::rename(input_path, target_path, ec);
    if (ec) {
        buildResponse(
            ResponseErrorInfos{
                .error_id = ResponseError::IOMoveError,
                .precision_msg = "error while moving file \"" + input_path.string() + "\" to \"" +
                                 target_path.string() + "\": " + ec.message(),
            },
            response);
        return false;
    }
    return true;
}

bool SFS::copy(const fs::path& input_path, fs::path output_path, ResponseBuilder* response) const {
    output_path = output_path.lexically_normal();
    const fs::path target_path{output_path / input_path.filename()};
    if (!beginCopyOrMove(input_path, target_path, response)) {
        return false;
    }
    std::error_code ec{};
    fs::copy(input_path, target_path, fs::copy_options::recursive | fs::copy_options::overwrite_existing, ec);
    if (ec) {
        buildResponse(
            ResponseErrorInfos{
                .error_id = ResponseError::IOCopyError,
                .precision_msg = "error while copying file \"" + input_path.string() + "\" to \"" +
                                 target_path.string() + "\": " + ec.message(),
            },
            response);
        return false;
    }
    return true;
}

bool SFS::beginCopyOrMove(const fs::path& input_path, const fs::path& target_path, ResponseBuilder* response) const {
    if (!canLexicallyAccess(input_path) || !canLexicallyAccess(target_path)) {
        buildResponse(ResponseErrorInfos{.error_id = ResponseError::InvalidPermissions}, response);
        return false;
    }
    if (!exists(input_path)) {
        buildResponse(
            ResponseErrorInfos{
                .error_id = ResponseError::IOFileNotExists,
                .precision_msg = "the file \"" + input_path.string() + "\" does not exist",
            },
            response);
        return false;
    }
    const fs::path dirs_to_create{target_path.parent_path()};
    std::error_code ec{};
    if (!fs::exists(dirs_to_create)) {
        fs::create_directories(dirs_to_create, ec);
    }
    if (ec) {
        buildResponse(
            ResponseErrorInfos{
                .error_id = ResponseError::IOCreateDirError,
                .precision_msg =
                    "error while creating directories \"" + dirs_to_create.string() + "\": " + ec.message(),
            },
            response);
        return false;
    }
    return true;
}

bool SFS::checkPathAlreadyExist(const fs::path& path, ResponseBuilder* response) const {
    if (exists(path) && response) {
        response->buildError(ResponseErrorInfos{.error_id = ResponseError::IOFileAlreadyExists});
    } else if (!canLexicallyAccess(path) && response) {
        response->buildError(ResponseErrorInfos{.error_id = ResponseError::InvalidPermissions});
    } else {
        return response != nullptr;
    }
    return false;
}

bool SFS::checkPathNotExist(const fs::path& path, ResponseBuilder* response) const {
    if (!exists(path) && response) {
        response->buildError(ResponseErrorInfos{.error_id = ResponseError::IOFileNotExists});
    } else if (!canLexicallyAccess(path) && response) {
        response->buildError(ResponseErrorInfos{.error_id = ResponseError::InvalidPermissions});
    } else {
        return response != nullptr;
    }
    return false;
}

void SFS::buildResponse(const ResponseErrorInfos& infos, ResponseBuilder* response) const {
    if (response) {
        response->buildError(infos);
    }
}

#pragma warning(pop)
