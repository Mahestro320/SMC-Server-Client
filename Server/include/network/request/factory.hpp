#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

#include "Shared/network/request/id.hpp"
#include "handler.hpp"
#include "handlers/get_version.hpp"
#include "handlers/io_copy_file.hpp"
#include "handlers/io_create_directory.hpp"
#include "handlers/io_create_file.hpp"
#include "handlers/io_delete_file.hpp"
#include "handlers/io_file_exists.hpp"
#include "handlers/io_get_dir_content.hpp"
#include "handlers/io_get_file_content.hpp"
#include "handlers/io_get_file_size.hpp"
#include "handlers/io_get_file_type.hpp"
#include "handlers/io_get_real_path.hpp"
#include "handlers/io_move_file.hpp"
#include "handlers/io_set_file_content.hpp"
#include "handlers/io_set_file_name.hpp"
#include "handlers/login.hpp"
#include "handlers/logout.hpp"
#include "handlers/restart_server.hpp"
#include "handlers/run_syscmd.hpp"

using rh_factory_t = std::function<std::unique_ptr<RH>()>;

static const std::unordered_map<RequestId, rh_factory_t> handler_map{
    {RequestId::GetVersion, []() { return std::make_unique<GetVersionRH>(); }},
    {RequestId::IOCreateDirectory, []() { return std::make_unique<IOCreateDirectoryRH>(); }},
    {RequestId::IOCreateFile, []() { return std::make_unique<IOCreateFileRH>(); }},
    {RequestId::IODeleteFile, []() { return std::make_unique<IODeleteFileRH>(); }},
    {RequestId::IOFileExists, []() { return std::make_unique<IOFileExistsRH>(); }},
    {RequestId::IOGetDirContent, []() { return std::make_unique<IOGetDirContentRH>(); }},
    {RequestId::IOGetFileContent, []() { return std::make_unique<IOGetFileContentRH>(); }},
    {RequestId::IOGetFileContent, []() { return std::make_unique<IOGetFileContentRH>(); }},
    {RequestId::IOGetFileSize, []() { return std::make_unique<IOGetFileSizeRH>(); }},
    {RequestId::IOGetFileType, []() { return std::make_unique<IOGetFileTypeRH>(); }},
    {RequestId::IOGetRealPath, []() { return std::make_unique<IOGetRealPathRH>(); }},
    {RequestId::IOMoveFile, []() { return std::make_unique<IOMoveFileRH>(); }},
    {RequestId::IOCopyFile, []() { return std::make_unique<IOCopyFileRH>(); }},
    {RequestId::IOSetFileContent, []() { return std::make_unique<IOSetFileContentRH>(); }},
    {RequestId::IOSetFileName, []() { return std::make_unique<IOSetFileNameRH>(); }},
    {RequestId::Login, []() { return std::make_unique<LoginRH>(); }},
    {RequestId::Logout, []() { return std::make_unique<LogoutRH>(); }},
    {RequestId::ServerRestart, []() { return std::make_unique<RestartServerRH>(); }},
    {RequestId::RunSyscmd, []() { return std::make_unique<RunSyscmdRH>(); }},
};
