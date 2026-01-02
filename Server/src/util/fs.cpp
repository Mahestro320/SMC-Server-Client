#include "util/fs.hpp"

#include <cctype>
#include <filesystem>
#include <string>

namespace stdfs = std::filesystem;

bool util::fs::haveDriveLetter(const stdfs::path& path) {
    const std::string str_path{path.string()};
    return str_path.size() >= 2 ? str_path[1] == ':' && std::isalpha(str_path[0]) : false;
}
