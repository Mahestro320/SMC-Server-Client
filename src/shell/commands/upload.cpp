#include "shell/commands/upload.hpp"

#include <filesystem>
#include <fstream>
#include <system_error>

#include "Shared/io/console.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/error_message.hpp"
#include "network/client.hpp"
#include "shell/commands/io_load/file_uploader.hpp"
#include "shell/exit_code.hpp"
#include "shell/lang.hpp"
#include "users/user.hpp"
#include "util/fs.hpp"

namespace fs = std::filesystem;

exit_code_t UploadCommand::run() {
    if (args.empty()) {
        return InvalidArgs;
    }
    getPathsFromArgs();
    if (!getInputFiles()) {
        return Error;
    }
    console::out::inf(lang::gt("command.upload.uploading_files", input_files.size()));
    for (const fs::path& path : input_files) {
        if (!openFile(path) || !uploadFile(path)) {
            closeFile();
            return Error;
        }
        closeFile();
    }
    return Success;
}

UploadCommand::~UploadCommand() {
    closeFile();
}

void UploadCommand::closeFile() {
    curr_input_file.close();
}

void UploadCommand::getPathsFromArgs() {
    input_path = args[0];
    input_path = input_path.lexically_normal();
    const User& user{client->getUser()};
    output_folder = user.current_dir;
    if (args.size() >= 2) {
        output_folder /= args[1];
    }
    output_folder = output_folder.lexically_normal();
}

bool UploadCommand::getInputFiles() {
    std::error_code ec{};
    if (!fs::exists(input_path, ec) || ec) {
        console::out::err(lang::gt("io.error.file_not_exists_prc", input_path.string()));
        return false;
    }
    if (fs::is_regular_file(input_path, ec)) {
        input_files.push_back(input_path);
        return true;
    }
    return util::fs::pushDirContentIn(input_files, input_path);
}

bool UploadCommand::openFile(const fs::path& path) {
    curr_input_file = std::ifstream{path, std::ios::binary | std::ios::ate};
    if (!curr_input_file.is_open() || !curr_input_file.good()) {
        console::out::inf(lang::gt("io.error.open_file", path.string(), util::error_message::get(errno)));
        return false;
    }
    curr_file_size = static_cast<pds::file_size_t>(curr_input_file.tellg());
    curr_input_file.seekg(0, std::ios::beg);
    return true;
}

bool UploadCommand::uploadFile(const fs::path& path) {
    fs::path output_path{};
    if (!getOutputPathFor(path, output_path)) {
        return false;
    }
    FileUploader uploader{};
    uploader.setClient(client);
    uploader.setStream(&curr_input_file);
    uploader.setStreamSize(curr_file_size);
    uploader.setTargetPath(output_path);
    return uploader.uploadFile();
}

bool UploadCommand::getOutputPathFor(const fs::path& path, fs::path& output_path) {
    std::error_code ec{};
    const fs::path target_path_relative{fs::relative(path, input_path.parent_path(), ec)};
    if (ec) {
        console::out::err(lang::gt("command.upload.error.while_getting_output_path", ec.message()));
        return false;
    }
    output_path = output_folder / target_path_relative;
    return true;
}
