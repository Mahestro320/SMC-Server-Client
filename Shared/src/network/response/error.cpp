#include "Shared/network/response/error.hpp"

#include <string>
#include <unordered_map>

namespace {

const std::unordered_map<ResponseError, std::string> response_names{
    {ResponseError::VersionTooOld, "VERSION_TOO_OLD"},
    {ResponseError::InvalidRequestData, "INVALID_REQUEST_DATA"},
    {ResponseError::UnknownRequest, "UNKNOWN_REQUEST"},

    {ResponseError::NotLogged, "NOT_LOGGED"},

    {ResponseError::InvalidPermissions, "INVALID_PERMISSIONS"},
    {ResponseError::InvalidPassword, "INVALID_PASSWORD"},
    {ResponseError::UserNotExists, "USER_NOT_EXISTS"},

    {ResponseError::UnknownRestartMode, "UNKNOWN_RESTART_MODE"},
    {ResponseError::OpenPipeError, "OPEN_PIPE_ERROR"},

    {ResponseError::IOInvalidFileName, "IO_INVALID_FILE_NAME"},
    {ResponseError::IOInvalidBufferSize, "IO_INVALID_BUFFER_SIZE"},
    {ResponseError::IOOpenFileError, "IO_OPEN_FILE_ERROR"},
    {ResponseError::IORenameError, "IO_RENAME_ERROR"},
    {ResponseError::IODeleteError, "IO_DELETE_ERROR"},
    {ResponseError::IOMoveError, "IO_MOVE_ERROR"},
    {ResponseError::IOCopyError, "IO_COPY_ERROR"},
    {ResponseError::IOCreateDirError, "IO_CREATE_DIR_ERROR"},
    {ResponseError::IOCreateFileError, "IO_CREATE_FILE_ERROR"},
    {ResponseError::IOFileAlreadyExists, "IO_FILE_ALREADY_EXISTS"},
    {ResponseError::IOFileNotExists, "IO_FILE_NOT_EXISTS"},
    {ResponseError::IOPathNotDirectory, "IO_PATH_NOT_DIRECTORY"},
};

}

std::string network::response_error::getName(ResponseError id) {
    return response_names.contains(id) ? response_names.at(id) : "UNKNOWN";
}
