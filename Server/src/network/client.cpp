#include "network/client.hpp"

#include <array>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/basic_endpoint.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/detail/error_code.hpp>
#include <exception>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/network/request/id.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/network/response/type.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/version.hpp"
#include "common.hpp"
#include "io/sfs.hpp"
#include "network.hpp"
#include "network/request/factory.hpp"
#include "network/request/handler.hpp"
#include "network/response/builder.hpp"
#include "server/config.hpp"
#include "users/user.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

thread_local std::unique_ptr<Client> Client::instance{nullptr};

Client::Client(tcp::socket socket, pds::client_count_t index) : socket{std::move(socket)}, index{index} {
}

void Client::setInstance(tcp::socket socket, pds::client_count_t index) {
    instance = std::make_unique<Client>(std::move(socket), index);
}

Client& Client::getInstance() {
    if (!instance) {
        throw std::runtime_error("client instance is not set");
    }
    return *instance;
}

void Client::start() {
    printLocation();
    try {
        startCommunication();
    } catch (std::exception& e) {
        console::out::err(e);
    }
    is_connected = false;
}

void Client::printLocation() const {
    console::out::inf("connected client at <:color=bright_blue>" + getAddress() + ':' + std::to_string(getPort()) +
                      "<:color=reset>");
}

void Client::startCommunication() {
    if (!checkWelcomeMessage()) {
        network::sendInt<bool>(socket, false);
        return;
    }
    if (!network::sendInt<bool>(socket, true)) {
        return;
    }
    checkVersion();
    while (is_connected && !common::shutdown_requested.load(std::memory_order_acquire)) {
        handleNewRequest();
    }
}

bool Client::checkWelcomeMessage() {
    std::array<char, WELCOME_MESSAGE_SIZE> buffer{};
    boost::system::error_code ec{};
    const size_t length{socket.read_some(boost::asio::buffer(buffer), ec)};
    if (ec) {
        console::out::err("error while reading welcome message: " + ec.message());
        return false;
    }
    if (length != WELCOME_MESSAGE_SIZE) {
        console::out::err("error while reading welcome message: invalid size");
        return false;
    }
    const std::string welcome_message{buffer.begin(), buffer.end()};
    if (!isWelcomeMessage(welcome_message)) {
        console::out::err("error while reading welcome message: invalid message: " + welcome_message);
        return false;
    }
    return true;
}

bool Client::isWelcomeMessage(const std::string& message) const {
    return message[0] == WELCOME_MESSAGE[0] && message[1] == WELCOME_MESSAGE[1] && message[2] == WELCOME_MESSAGE[2];
}

void Client::checkVersion() {
    pds::version_t version_int{};
    if (!network::readInt<pds::version_t>(socket, version_int)) {
        return;
    }
    version.setFromCombined(version_int);
    console::out::inf("client version: <:color=green>" + version.toString() + "<:color=reset> (<:color=green>" +
                      std::to_string(version_int) + "<:color=reset>)");
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    network::sendInt<pds::version_t>(socket, common::VERSION.combine()) &&
        network::sendInt<pds::version_t>(socket, config_values.client_min_version.combine()) &&
        network::sendInt<pds::version_t>(socket, config_values.client_max_version.combine());
}

void Client::handleNewRequest() {
    std::vector<char> request_full_content{};
    if (!network::readBuffer(socket, request_full_content, true)) {
        is_connected = false;
        return;
    }
    if (request_full_content.empty()) {
        console::out::err("the request content is empty");
        return;
    }
    const pds::request_id_t request_id_int{static_cast<pds::request_id_t>(request_full_content[0])};
    const RequestId request_id{static_cast<RequestId>(request_id_int)};
    console::out::inf("received request: <:color=bright_blue>" + std::to_string(request_id_int) +
                      "<:color=reset> (<:color=green>" + network::request::getName(request_id) + "<:color=reset>)");
    const std::span<char> request_content{request_full_content.begin() + 1, request_full_content.size() - 1};
    try {
        processReceivedRequest(request_id, request_content);
    } catch (const std::exception& e) {
        console::out::err(e);
    }
}

void Client::processReceivedRequest(RequestId request, const std::span<char>& request_content) {
    if (request == RequestId::Quit) {
        console::out::inf("goodbye !");
        is_connected = false;
        return;
    }
    const std::unique_ptr<RH> handler{getHandlerFromRequest(request)};
    if (!handler) {
        sendResponseError(ResponseError::UnknownRequest);
        return;
    }
    handler->setClient(this);
    handler->setRequestContent(request_content);
    try {
        handler->run();
    } catch (const std::exception& e) {
        console::out::err("error while processing request" + std::to_string(static_cast<pds::request_id_t>(request)) +
                          ": " + e.what());
    }
}

void Client::sendResponseError(ResponseError err) {
    ResponseBuilder response{};
    response.buildError(ResponseErrorInfos{
        .error_id = err,
    });
    network::sendBuffer(socket, response.getResult());
}

std::unique_ptr<RH> Client::getHandlerFromRequest(RequestId request) const {
    auto it = handler_map.find(request);
    if (it != handler_map.end()) {
        return it->second();
    }
    return nullptr;
}

bool Client::isLogged() const {
    return is_logged;
}

tcp::socket& Client::getSocket() {
    return socket;
}

const User& Client::getUser() const {
    return user;
}

SFS& Client::getSFS() {
    return sfs;
}

pds::client_count_t Client::getIndex() const {
    return index;
}

pds::connection_port_t Client::getPort() const {
    const tcp::endpoint remote_endpoint{getRemoteEndpoint()};
    const boost::asio::ip::port_type port{remote_endpoint.port()};
    return port;
}

std::string Client::getAddress() const {
    const tcp::endpoint remote_endpoint{getRemoteEndpoint()};
    const boost::asio::ip::address address{remote_endpoint.address()};
    return address.to_string();
}

const Version& Client::getVersion() const {
    return version;
}

tcp::endpoint Client::getRemoteEndpoint() const {
    return socket.remote_endpoint();
}

void Client::setUser(const User& user) {
    this->user = user;
}

void Client::setLogged(bool is_logged) {
    this->is_logged = is_logged;
}
