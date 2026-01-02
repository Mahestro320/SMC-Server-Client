#include "util/fs.hpp"

#include <filesystem>
#include <string>
#include <system_error>
#include <vector>

#include "Shared/io/console.hpp"
#include "shell/lang.hpp"

std::filesystem::path util::fs::getNonExistingPath(const std::filesystem::path& path) {
    std::filesystem::path output{};
    size_t i{};
    const std::filesystem::path norm_path{path.lexically_normal()};
    const std::filesystem::path parent_path{norm_path.parent_path()};
    const std::filesystem::path filename_no_ext{norm_path.stem()};
    const std::filesystem::path ext{norm_path.extension()};
    do {
        output = parent_path / std::filesystem::path{filename_no_ext.string() +
                                                     ((i == 0) ? "" : '_' + std::to_string(i + 1)) + ext.string()};
        ++i;
    } while (std::filesystem::exists(output));
    return output;
}

bool util::fs::pushDirContentIn(std::vector<std::filesystem::path>& vec, const std::filesystem::path& path) {
    if (!std::filesystem::is_directory(path)) {
        console::out::err(lang::gt("io.error.path_not_directory", path.string()));
        return false;
    }
    std::error_code ec{};
    for (const std::filesystem::path& current : std::filesystem::directory_iterator{path, ec}) {
        if (std::filesystem::is_regular_file(current, ec)) {
            vec.push_back(path / current);
        } else if (std::filesystem::is_directory(current, ec)) {
            if (!pushDirContentIn(vec, path / current)) {
                return false;
            }
        } else {
            console::out::err(lang::gt("io.error.unhandled_file_type"));
            return false;
        }
    }
    if (ec) {
        console::out::err(lang::gt("io.error.no_message", ec.message()));
        return false;
    }
    return true;
}
