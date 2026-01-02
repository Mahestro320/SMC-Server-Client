#include "Shared/network/array_editor.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include "Shared/pds.hpp"

namespace fs = std::filesystem;

void ArrayAnalyzer::setSizeError(const std::string& extract_target, size_t size_required) {
    last_error = "error while extracting " + extract_target + ": needed " + std::to_string(cursor + size_required) +
                 " bytes, got " + std::to_string(data.size()) + " bytes";
}

void ArrayAnalyzer::setCursor(size_t cursor) {
    this->cursor = cursor;
}

void ArrayAnalyzer::setData(std::vector<char>&& data) {
    this->cursor = 0;
    this->data = std::move(data);
}

void ArrayAnalyzer::clearError() {
    last_error.clear();
}

const std::string& ArrayAnalyzer::getLastError() const {
    return last_error;
}

size_t ArrayAnalyzer::getSize() const {
    return data.size();
}

bool ArrayAnalyzer::isError() const {
    return !last_error.empty();
}

bool ArrayAnalyzer::extractBool(bool& output) {
    pds::bool_t byte{};
    if (!extractInt<pds::bool_t>(byte)) {
        return false;
    }
    output = (byte != 0);
    return true;
}

bool ArrayAnalyzer::extractPath(fs::path& path) {
    std::string path_str{};
    if (!extractString<pds::path_t>(path_str)) {
        return false;
    }
    path = fs::path{path_str};
    return true;
}

void ArrayBuilder::insertBool(bool value) {
    insertInt<pds::bool_t>(value ? 1 : 0);
}

void ArrayBuilder::insertPath(const fs::path& path) {
    insertString<pds::path_t>(path.generic_string());
}

const std::vector<char>& ArrayBuilder::getResult() const {
    return data;
}
