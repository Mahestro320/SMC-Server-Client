#include "network/request/handler.hpp"

#include <span>
#include <string>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/network/response/type.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/version.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "server/config.hpp"
#include "users/user.hpp"

RH::RH(const RHRequirements& req) : requirements{req} {
}

void RH::setClient(Client* client) {
    this->client = client;
}

void RH::setRequestContent(const std::span<char>& content) {
    request.setData(std::vector<char>{content.begin(), content.end()});
}

void RH::run() {
    response.buildNormal();
    checkVersion() && checkMustBeLogged() && checkMinRole();
    if (response.getType() == ResponseType::Ok) {
        _run();
    }
    if (request.isError()) {
        response.buildError(ResponseErrorInfos{
            .error_id = ResponseError::InvalidRequestData,
            .precision_msg = request.getLastError(),
        });
    }
    if (!response_sent) {
        sendResponse();
    }
}

bool RH::sendResponse() {
    if (response_sent) {
        console::out::err("response has already been sent");
        return false;
    }
    response_sent = true;
    if (response.getType() == ResponseType::Error) {
        const ResponseError error_id{response.getError()};
        std::string error_msg{"sending error response with id " + network::response_error::getName(error_id) + " (" +
                              std::to_string(static_cast<pds::response_error_t>(error_id)) + ')'};
        if (response.isErrorMessage()) {
            error_msg += ": " + response.getErrorMessage();
        }
        console::out::err(error_msg);
    }
    return network::sendBuffer(client->getSocket(), response.getResult(), true);
}

bool RH::checkVersion() {
    const Version& client_ver{client->getVersion()};
    const pds::version_t client_ver_combined{client_ver.combine()};

    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};

    if (client_ver_combined < requirements.min_version.combine()) {
        console::out::err("the client version is too old");
        response.buildError(ResponseErrorInfos{.error_id = ResponseError::VersionTooOld});
        return false;
    } else if (client_ver_combined > config_values.client_max_version.combine()) {
        console::out::warn("the client version is too recent");
    }
    return true;
}

bool RH::checkMustBeLogged() {
    if (requirements.must_be_logged && !client->isLogged()) {
        console::out::err("the client must be logged to run this request");
        response.buildError(ResponseErrorInfos{.error_id = ResponseError::NotLogged});
        return false;
    }
    return true;
}

bool RH::checkMinRole() {
    const User& user{client->getUser()};
    if (user.role < requirements.min_role) {
        console::out::err("invalid user permissions");
        response.buildError(ResponseErrorInfos{.error_id = ResponseError::InvalidPermissions});
        return false;
    }
    return true;
}
