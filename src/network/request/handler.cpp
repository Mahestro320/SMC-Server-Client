#include "network/request/handler.hpp"

#include <utility>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/network/request/id.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/network/response/type.hpp"
#include "Shared/pds.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "shell/lang.hpp"

RH::RH(RequestId id) : id{id} {
}

bool RH::sendRequest() {
    return network::sendBuffer(client->getSocket(), request.getResult());
}

bool RH::checkResponse() {
    if (!readResponse()) {
        return false;
    }
    pds::response_type_t response_type{};
    if (!response.extractInt<pds::response_type_t>(response_type)) {
        printResponseLastError();
        return false;
    }
    if (response_type == static_cast<pds::response_type_t>(ResponseType::Error)) {
        processResponseError();
        return false;
    } else if (response_type == static_cast<pds::response_type_t>(ResponseType::Ok)) {
        return true;
    }
    console::out::err(lang::gt("network.error.unknown_response_type", response_type));
    return false;
}

bool RH::readResponse() {
    std::vector<char> buffer{};
    if (!network::readBuffer(client->getSocket(), buffer)) {
        return false;
    }
    response.setData(std::move(buffer));
    return true;
}

void RH::processResponseError() {
    ResponseErrorInfos infos{};
    pds::response_error_t error_id{};
    if (!response.extractInt<pds::response_error_t>(error_id)) {
        printResponseLastError();
        return;
    }
    infos.error_id = static_cast<ResponseError>(error_id);
    if (!response.extractString<pds::response_msg_t>(infos.precision_msg)) {
        printResponseLastError();
        return;
    }
    response.clearError();
    printResponseError(infos);
}

void RH::printResponseLastError() {
    console::out::err(lang::gt("array_editor.error", response.getLastError()));
}

void RH::printResponseError(const ResponseErrorInfos& infos) {
    console::out::err(lang::gt("network.error.server_returned", static_cast<pds::response_error_t>(infos.error_id),
                               network::response_error::getName(infos.error_id)));
    if (!infos.precision_msg.empty()) {
        console::out::err(infos.precision_msg);
    }
}

bool RH::run() {
    request.insertInt<pds::request_id_t>(static_cast<pds::request_id_t>(id));
    const bool success{_run()};
    if (!success && response.isError()) {
        console::out::err(lang::gt("network.error.invalid_response", response.getLastError()));
    }
    return success;
}
