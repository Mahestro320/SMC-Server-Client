#pragma once

#include <string>

#include "Shared/pds.hpp"

enum class ResponseError : pds::response_error_t {
    // Global
    VersionTooOld = 0x00,
    InvalidRequestData,
    UnknownRequest,

    // Client state
    NotLogged = 0x10,

    // User
    InvalidPermissions = 0x20,
    InvalidPassword,
    UserNotExists,

    // Others
    UnknownRestartMode = 0x30,
    OpenPipeError,

    // IO Errors
    IOInvalidFileName = 0x40,
    IOInvalidBufferSize,
    IOOpenFileError,
    IORenameError,
    IODeleteError,
    IOMoveError,
    IOCopyError,
    IOCreateDirError,
    IOCreateFileError,
    IOFileAlreadyExists,
    IOFileNotExists,
    IOPathNotDirectory,
};

struct ResponseErrorInfos {
    ResponseError error_id{};
    std::string precision_msg{};
};

namespace network::response_error {

extern std::string getName(ResponseError id);

}
