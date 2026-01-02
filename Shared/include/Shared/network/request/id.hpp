#pragma once

#include <string>

#include "Shared/pds.hpp"

enum class RequestId : pds::request_id_t {
    GetVersion = 0x00,

    // Global others
    Quit = 0x20,
    Login,
    Logout,
    RunSyscmd,

    // IO getters
    IOFileExists = 0x40,
    IOGetDirContent,
    IOGetFileType,
    IOGetFileSize,
    IOGetFileContent,
    IOGetRealPath,

    // IO setters
    IOSetFileContent = 0x60,
    IOSetFileName,

    // IO others
    IOCreateFile = 0x80,
    IOCreateDirectory,
    IODeleteFile,
    IOMoveFile,
    IOCopyFile,

    // Server
    ServerRestart = 0xA0,
};

namespace network::request {

extern std::string getName(RequestId id);

} // namespace network::request
