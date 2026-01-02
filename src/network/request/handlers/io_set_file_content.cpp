#include "network/request/handlers/io_set_file_content.hpp"

#include <filesystem>
#include <ios>
#include <istream>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/network/request/id.hpp"
#include "Shared/pds.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "network/request/handler.hpp"
#include "shell/config.hpp"
#include "shell/lang.hpp"

namespace fs = std::filesystem;

IOSetFileContentRH::IOSetFileContentRH() : RH{RequestId::IOSetFileContent} {
}

void IOSetFileContentRH::setStream(std::istream* stream) {
    this->stream = stream;
}

void IOSetFileContentRH::setStreamSize(pds::file_size_t size) {
    this->size = size;
}

void IOSetFileContentRH::setPath(const fs::path& path) {
    this->path = path;
}

bool IOSetFileContentRH::_run() {
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    buffer_size = config_values.cmd_upload_buffer_size;

    calcBufferCount();

    request.insertPath(path);
    request.insertInt<pds::file_size_t>(size);
    request.insertInt<pds::network_buffer_t>(buffer_size);
    request.insertInt<pds::network_buffer_count_t>(buffer_count);
    return sendRequest() && checkResponse();
}

void IOSetFileContentRH::calcBufferCount() {
    buffer_count = size / buffer_size + 1;
}

bool IOSetFileContentRH::isAvailable() const {
    return curr_idx < buffer_count;
}

pds::network_buffer_count_t IOSetFileContentRH::getBufferCount() const {
    return buffer_count;
}

pds::network_buffer_count_t IOSetFileContentRH::getCurrentBufferIndex() const {
    return curr_idx;
}

pds::network_buffer_t IOSetFileContentRH::getBufferSize() {
    return buffer_size;
}

pds::file_size_t IOSetFileContentRH::getFileSize() {
    return size;
}

bool IOSetFileContentRH::uploadNextBuffer() {
    if (!checkStream()) {
        return false;
    }
    if (!isAvailable()) {
        console::out::err(lang::gt("command.io_load.error.in.no_more_data"));
        return false;
    }
    std::vector<char> buffer{};
    pds::network_buffer_t curr_buffer_size{buffer_size};
    if (curr_idx == buffer_count - 1) {
        curr_buffer_size = size % buffer_size;
    }
    buffer.resize(curr_buffer_size);
    stream->read(buffer.data(), static_cast<std::streamsize>(curr_buffer_size));
    if (!network::sendBuffer(client->getSocket(), buffer)) {
        return false;
    }
    curr_idx++;
    return true;
}

bool IOSetFileContentRH::checkStream() const {
    if (!stream) {
        console::out::err(lang::gt("command.io_load.error.in.stream_nullptr"));
        return false;
    }
    return true;
}
