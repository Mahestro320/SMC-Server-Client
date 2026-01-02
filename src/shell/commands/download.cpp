#include "shell/commands/download.hpp"

#include <filesystem>
#include <fstream>
#include <system_error>
#include <utility>

#include "Shared/io/console.hpp"
#include "Shared/io/file_type.hpp"
#include "Shared/util/error_message.hpp"
#include "network/client.hpp"
#include "network/request/handlers/io_get_file_type.hpp"
#include "network/request/handlers/util/get_dir_content_recursively.hpp"
#include "shell/commands/io_load/file_downloader.hpp"
#include "shell/config.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"
#include "users/user.hpp"

namespace fs = std::filesystem;

exit_code_t DownloadCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    getInputPath();
    if (!getInputFiles()) {
        return Error;
    }
    console::out::inf(lang::gt("command.download.downloading_files", input_files.size()));
    for (const fs::path& path : input_files) {
        if (!initStreamFor(path) || !downloadFile(path)) {
            closeFile();
            return Error;
        }
        closeFile();
    }
    return Success;
}

DownloadCommand::~DownloadCommand() {
    closeFile();
}

void DownloadCommand::closeFile() {
    curr_output_stream.close();
}

void DownloadCommand::getInputPath() {
    const User& user{client->getUser()};
    input_path = user.current_dir / args[0];
    input_path = input_path.lexically_normal();
}

bool DownloadCommand::getInputFiles() {
    IOGetFileTypeRH handler{};
    handler.setClient(client);
    handler.setPath(input_path);
    if (!handler.run()) {
        return false;
    }
    const FileType type{handler.getValue()};
    if (type == FileType::File) {
        input_files.push_back(input_path);
        return true;
    } else if (type == FileType::Directory) {
        return getInputFilesFromFolder();
    }
    console::out::err(lang::gt("io.error.file_not_exists_prc", input_path.string()));
    return false;
}

bool DownloadCommand::initStreamFor(const fs::path& path) {
    fs::path output_path{};
    if (!getOutputPathFor(path, output_path)) {
        return false;
    }
    const fs::path dir_to_create{output_path.parent_path()};
    std::error_code ec{};
    fs::create_directories(dir_to_create, ec);
    (void)ec;

    curr_output_stream = std::ofstream{output_path, std::ios::binary | std::ios::beg};
    if (!curr_output_stream.is_open() || !curr_output_stream.good()) {
        console::out::err(lang::gt("io.error.open_file", output_path.string(), util::error_message::get(errno)));
        return false;
    }
    return true;
}

bool DownloadCommand::getOutputPathFor(const fs::path& path, fs::path& output) {
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    const fs::path output_folder{config_values.cmd_download_output};
    std::error_code ec{};
    const fs::path target_path_relative{fs::relative(path, input_path.parent_path(), ec)};
    if (ec) {
        console::out::err(lang::gt("command.download.error.while_getting_output_path", ec.message()));
        return false;
    }
    output = output_folder / target_path_relative;
    return true;
}

bool DownloadCommand::downloadFile(const fs::path& path) {
    FileDownloader downloader{};
    downloader.setClient(client);
    downloader.setTargetPath(path);
    downloader.setStream(&curr_output_stream);
    return downloader.downloadFile();
}

bool DownloadCommand::getInputFilesFromFolder() {
    GetDirContentRecursively handler{};
    handler.setClient(client);
    handler.setPath(input_path);
    if (!handler.run()) {
        return false;
    }
    input_files = std::move(handler.getContent());
    return true;
}
