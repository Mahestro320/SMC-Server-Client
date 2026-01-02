#include "network/bfl/decoder.hpp"

#include <string>
#include <utility>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/io/file_info.hpp"
#include "Shared/io/file_type.hpp"
#include "Shared/pds.hpp"
#include "shell/lang.hpp"

namespace {

void printInvalidSizeError(const std::string& location) {
    console::out::err(lang::gt("bfl.decoder.error.invalid_size", location));
}

} // namespace

void BFLDecoder::printVerbose(const std::string& message, bool end_line) {
    if (!is_silent) {
        console::out::verbose(message, end_line);
    }
}

void BFLDecoder::setSilent(bool silent) {
    this->is_silent = silent;
}

void BFLDecoder::setBFL(std::vector<char>&& bfl) {
    this->bfl = std::move(bfl);
}

bool BFLDecoder::decode() {
    printVerbose(lang::gt("bfl.decoder.decode_data"), false);
    if (!getFileCount()) {
        return false;
    }
    for (size_t i{0}; i < file_count; ++i) {
        if (!decodeFile()) {
            return false;
        }
    }
    printVerbose(lang::gt("global.done"));
    return true;
}

bool BFLDecoder::getFileCount() {
    if (bfl.size() < sizeof(pds::file_count_t)) {
        printInvalidSizeError("while decoding file count");
        return false;
    }
    std::memcpy(&file_count, bfl.data() + current_offset, sizeof(pds::file_count_t));
    current_offset += sizeof(pds::file_count_t);
    return true;
}

bool BFLDecoder::decodeFile() {
    FileInfo file_info{};
    if (!getFileType(file_info.type) || !getFileSize(file_info.size) || !getFileName(file_info.name)) {
        return false;
    }
    data.push_back(file_info);
    return true;
}

bool BFLDecoder::getFileType(FileType& type) {
    if (bfl.size() < current_offset + sizeof(pds::file_type_t)) {
        printInvalidSizeError("while decoding file type");
        return false;
    }
    std::memcpy(&type, bfl.data() + current_offset, sizeof(pds::file_type_t));
    current_offset += sizeof(pds::file_type_t);
    return true;
}

bool BFLDecoder::getFileSize(pds::file_size_t& size) {
    if (bfl.size() < current_offset + sizeof(pds::file_size_t)) {
        printInvalidSizeError("while decoding file size");
        return false;
    }
    std::memcpy(&size, bfl.data() + current_offset, sizeof(pds::file_size_t));
    current_offset += sizeof(pds::file_size_t);
    return true;
}

bool BFLDecoder::getFileName(std::string& name) {
    if (bfl.size() < current_offset + sizeof(pds::file_name_t)) {
        printInvalidSizeError("while decoding file name size");
        return false;
    }
    pds::file_name_t name_size{};
    std::memcpy(&name_size, bfl.data() + current_offset, sizeof(pds::file_name_t));
    current_offset += sizeof(pds::file_name_t);
    if (bfl.size() < current_offset + name_size) {
        printInvalidSizeError("while decoding file name content");
        return false;
    }
    name = std::string{bfl.data() + current_offset, name_size};
    current_offset += name_size;
    return true;
}

const std::vector<FileInfo>& BFLDecoder::getData() const {
    return data;
}
