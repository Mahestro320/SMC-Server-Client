#include "users/uif/decoder.hpp"

#include <filesystem>
#include <fstream>
#include <string>

#include "Shared/io/console.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/string.hpp"
#include "io/directories.hpp"
#include "users/uif/values.hpp"

namespace fs = std::filesystem;

bool UIFDecoder::loadDataFromPath(const fs::path& path) {
    std::ifstream file{path, std::ios::binary | std::ios::ate};
    const fs::path relative_path{fs::relative(path, dirs::strg)};
    if (!file) {
        console::out::err("error while opening UIF file");
        return false;
    }
    const std::streampos file_size{file.tellg()};
    file.seekg(std::ios::beg);
    data.resize(static_cast<size_t>(file_size));
    if (!file.read(data.data(), file_size)) {
        console::out::err("error while reading UIF file");
        return false;
    }
    return true;
}

bool UIFDecoder::decode() {
    if (!checkSize() || !checkSignature()) {
        return false;
    }
    getRole();
    getUsername();
    if (values.username.empty()) {
        console::out::err("the username is empty");
        return false;
    }
    getPassword();
    return true;
}

bool UIFDecoder::checkSize() const {
    const pds::file_size_t data_size{static_cast<pds::file_size_t>(data.size())};
    if (data_size < DATA_SIZE) {
        console::out::err("invalid UIF data size: " + std::to_string(data_size) + " (must be " +
                          std::to_string(DATA_SIZE) + ")");
        return false;
    }
    return true;
}

bool UIFDecoder::checkSignature() const {
    if (data[SIGNATURE_OFFSET + 0] != SIGNATURE[0] || data[SIGNATURE_OFFSET + 1] != SIGNATURE[1] ||
        data[SIGNATURE_OFFSET + 2] != SIGNATURE[2] || data[SIGNATURE_OFFSET + 3] != SIGNATURE[3]) {
        console::out::err("invalid file signature");
        return false;
    }
    return true;
}

void UIFDecoder::getRole() {
    values.role = static_cast<pds::role_t>(data[ROLE_OFFSET]);
}

void UIFDecoder::getUsername() {
    values.username = util::string::getStringFromArray(data.data() + USERNAME_BLOCK_OFFSET, USERNAME_BLOCK_SIZE);
}

void UIFDecoder::getPassword() {
    values.password = util::string::getStringFromArray(data.data() + PASSWORD_BLOCK_OFFSET, PASSWORD_BLOCK_SIZE);
}

const UIFValues& UIFDecoder::getValues() const {
    return values;
}
