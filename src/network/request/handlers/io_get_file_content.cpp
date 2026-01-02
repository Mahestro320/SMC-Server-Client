#include "network/request/handlers/io_get_file_content.hpp"

#include <filesystem>
#include <ios>
#include <ostream>
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

IOGetFileContentRH::IOGetFileContentRH() : RH{RequestId::IOGetFileContent} {
}

void IOGetFileContentRH::setStream(std::ostream* stream) {
    this->stream = stream;
}

void IOGetFileContentRH::setPath(const fs::path& path) {
    this->path = path;
}

bool IOGetFileContentRH::_run() {
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    buffer_size = config_values.cmd_download_buffer_size;
    request.insertPath(path);
    request.insertInt<pds::network_buffer_t>(buffer_size);
    return sendRequest() && checkResponse() && extractFileSize() && extractBufferCount();
}

bool IOGetFileContentRH::extractFileSize() {
    return response.extractInt<pds::file_size_t>(size);
}

bool IOGetFileContentRH::extractBufferCount() {
    return response.extractInt<pds::network_buffer_count_t>(buffer_count);
}

bool IOGetFileContentRH::isAvailable() const {
    return curr_idx < buffer_count;
}

pds::network_buffer_count_t IOGetFileContentRH::getBufferCount() const {
    return buffer_count;
}

pds::network_buffer_count_t IOGetFileContentRH::getCurrentBufferIndex() const {
    return curr_idx;
}

pds::network_buffer_t IOGetFileContentRH::getBufferSize() {
    return buffer_size;
}

pds::file_size_t IOGetFileContentRH::getFileSize() {
    return size;
}

bool IOGetFileContentRH::downloadNextBuffer() {
    if (!checkStream()) {
        return false;
    }
    if (!isAvailable()) {
        console::out::err(lang::gt("command.io_load.error.out.no_more_data"));
        return false;
    }
    std::vector<char> buffer{};
    pds::network_buffer_t curr_buffer_size{buffer_size};
    if (curr_idx == buffer_count - 1) {
        curr_buffer_size = size % buffer_size;
    }
    buffer.resize(curr_buffer_size);
    if (!network::readBuffer(client->getSocket(), buffer)) {
        return false;
    }
    stream->write(buffer.data(), static_cast<std::streamsize>(curr_buffer_size));
    curr_idx++;
    return true;
}

bool IOGetFileContentRH::checkStream() const {
    if (!stream) {
        console::out::err(lang::gt("command.io_load.error.out.stream_nullptr"));
        return false;
    }
    return true;
}
