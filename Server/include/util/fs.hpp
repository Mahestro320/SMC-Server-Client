#pragma once

#include <filesystem>

namespace util::fs {

extern bool haveDriveLetter(const std::filesystem::path& path);

} // namespace util::fs
