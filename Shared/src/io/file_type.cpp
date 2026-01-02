#include "Shared/io/file_type.hpp"

#include <string>
#include <unordered_map>

#include "Shared/pds.hpp"

namespace {

std::unordered_map<FileType, std::string> file_type_names{
    {FileType::None, "NONE"},
    {FileType::File, "FILE"},
    {FileType::Directory, "DIRECTORY"},
};

}

std::string file_type::getName(FileType id) {
    return file_type_names.contains(id) ? file_type_names.at(id) : "UNKNOWN";
}

bool file_type::isValid(FileType id) {
    return static_cast<pds::file_type_t>(id) <= 3;
}
