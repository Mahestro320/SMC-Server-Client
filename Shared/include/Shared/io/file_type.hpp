#pragma once

#include <string>

#include "Shared/pds.hpp"

enum class FileType : pds::file_type_t {
    None,
    File,
    Directory,
};

namespace file_type {

extern std::string getName(FileType id);
extern bool isValid(FileType id);

} // namespace file_type
