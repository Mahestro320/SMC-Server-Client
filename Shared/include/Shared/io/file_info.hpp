#pragma once

#include <string>

#include "Shared/pds.hpp"
#include "file_type.hpp"

struct FileInfo {
    std::string name{};
    FileType type{};
    pds::file_size_t size{};
};
