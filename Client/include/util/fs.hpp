#pragma once

#include <filesystem>
#include <vector>

namespace util::fs {

extern std::filesystem::path getNonExistingPath(const std::filesystem::path& path);
extern bool pushDirContentIn(std::vector<std::filesystem::path>& vec, const std::filesystem::path& path);

} // namespace util::fs
