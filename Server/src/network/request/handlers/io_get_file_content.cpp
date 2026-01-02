#include "network/request/handlers/io_get_file_content.hpp"

#include <cstdlib>
#include <string>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/error_message.hpp"
#include "network.hpp"
#include "network/request/handler.hpp"
#include "server/config.hpp"

IOGetFileContentRH::IOGetFileContentRH() : RH{REQUIREMENTS} {
}

IOGetFileContentRH::~IOGetFileContentRH() {
    closeFile();
}

void IOGetFileContentRH::_run() {
    if (!extractPath() || !extractBufferSize() || !checkBufferSize() || !openFile()) {
        return;
    }
    insertFileSize();
    insertBufferCount();
    if (!sendResponse()) {
        closeFile();
        return;
    }
    transmitData();
    closeFile();
}

bool IOGetFileContentRH::extractPath() {
    return request.extractAbsPath(*client, target_path, response);
}

bool IOGetFileContentRH::extractBufferSize() {
    return request.extractInt<pds::network_buffer_t>(buffer_size);
}

bool IOGetFileContentRH::checkBufferSize() {
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    if (buffer_size == 0 || buffer_size > config_values.io_load_max_buffer_size) {
        response.buildError(ResponseErrorInfos{
            .error_id = ResponseError::IOInvalidBufferSize,
            .precision_msg = "invalid buffer size: " + std::to_string(buffer_size) + " (minimum is 1, maximum is " +
                             std::to_string(config_values.io_load_max_buffer_size) + ")",
        });
        return false;
    }
    return true;
}

bool IOGetFileContentRH::openFile() {
    file.open(target_path, std::ios::binary | std::ios::ate);
    if (!file.is_open() || !file.good()) {
        response.buildError(ResponseErrorInfos{
            .error_id = ResponseError::IOOpenFileError,
            .precision_msg = "cannot open file \"" + target_path.string() + "\": " + util::error_message::get(errno),
        });
        return false;
    }
    file_size = static_cast<pds::file_size_t>(file.tellg());
    file.seekg(0, std::ios::beg);
    buffer_count = file_size / buffer_size + 1;
    return true;
}

void IOGetFileContentRH::insertFileSize() {
    response.insertInt<pds::file_size_t>(file_size);
}

void IOGetFileContentRH::insertBufferCount() {
    response.insertInt<pds::network_buffer_count_t>(buffer_count);
}

bool IOGetFileContentRH::transmitData() {
    console::out::inf("sending " + std::to_string(buffer_count) + " buffers");
    std::vector<char> buffer(buffer_size);
    for (pds::network_buffer_count_t i{}; i < buffer_count; ++i) {
        if (i == buffer_count - 1 && file_size % buffer_size != 0) {
            buffer.resize(static_cast<size_t>(file_size % buffer_size));
        }
        file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
        if (!network::sendBuffer(client->getSocket(), buffer, true)) {
            return false;
        }
    }
    return true;
}

void IOGetFileContentRH::closeFile() {
    if (file.is_open()) {
        file.close();
    }
}
