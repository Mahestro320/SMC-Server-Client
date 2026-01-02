#include "network/bfl/encoder.hpp"

#include <string>
#include <utility>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/io/file_info.hpp"
#include "Shared/pds.hpp"

void BFLEncoder::setData(std::vector<FileInfo>&& data) {
    this->data = std::move(data);
}

void BFLEncoder::encode() {
    console::out::inf("encoding a list of " + std::to_string(data.size()) + " files");
    pushFileCount();
    allocMemoryForFiles();
    for (size_t i{}; i < data.size(); ++i) {
        pushFile(data[i]);
    }
}

void BFLEncoder::pushFileCount() {
    bfl.resize(current_offset + sizeof(pds::file_count_t));
    const pds::file_count_t data_size{static_cast<pds::file_count_t>(data.size())};
    std::memcpy(bfl.data() + current_offset, &data_size, sizeof(pds::file_count_t));
    current_offset += sizeof(pds::file_count_t);
}

void BFLEncoder::allocMemoryForFiles() {
    size_t final_size{};
    for (const FileInfo& file : data) {
        final_size += getFileSizeInBuffer(file);
    }
    bfl.resize(current_offset + final_size);
}

void BFLEncoder::pushFile(const FileInfo& file) {
    pushFileTypeOf(file);
    pushFileSizeOf(file);
    pushFileNameSizeOf(file);
    pushFileNameOf(file);
}

void BFLEncoder::pushFileTypeOf(const FileInfo& file) {
    const pds::file_type_t file_type{static_cast<pds::file_type_t>(file.type)};
    std::memcpy(bfl.data() + current_offset, &file_type, sizeof(pds::file_type_t));
    current_offset += sizeof(pds::file_type_t);
}

void BFLEncoder::pushFileSizeOf(const FileInfo& file) {
    std::memcpy(bfl.data() + current_offset, &file.size, sizeof(pds::file_size_t));
    current_offset += sizeof(pds::file_size_t);
}

void BFLEncoder::pushFileNameSizeOf(const FileInfo& file) {
    const pds::file_name_t file_name_size{static_cast<pds::file_name_t>(file.name.size())};
    std::memcpy(bfl.data() + current_offset, &file_name_size, sizeof(pds::file_name_t));
    current_offset += sizeof(pds::file_name_t);
}

void BFLEncoder::pushFileNameOf(const FileInfo& file) {
    std::memcpy(bfl.data() + current_offset, file.name.data(), file.name.size());
    current_offset += file.name.size();
}

pds::file_size_t BFLEncoder::getFileSizeInBuffer(const FileInfo& file) {
    // [file type][file size][filename size][filename content]
    return sizeof(pds::file_type_t) + sizeof(pds::file_size_t) + sizeof(pds::file_name_t) + file.name.size();
}

const std::vector<char>& BFLEncoder::getBFL() const {
    return bfl;
}
