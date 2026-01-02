#include "shell/commands/io_load/file_loader.hpp"

#include <filesystem>

void FileLoader::setTargetPath(const std::filesystem::path& path) {
    this->target_path = path;
}
