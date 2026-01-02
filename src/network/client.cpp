#include "network/client.hpp"

#include <boost/asio.hpp>
#include <string>

#include "Shared/io/console.hpp"
#include "Shared/network/request/id.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/version.hpp"
#include "constants.hpp"
#include "network.hpp"
#include "shell/config.hpp"
#include "users/user.hpp"

using boost::asio::ip::tcp;

Client::~Client() {
    socket.close();
}

void Client::resetState() {
    user = User{};
    is_logged = false;
    is_connected = false;
}

bool Client::connect() {
    resetState();
    is_connected = connectToServer();
    if (!is_connected) {
        return false;
    }
    if (!checkWelcomeMessage()) {
        console::out::err(lang::gt("client_class.error.disconnect_from_server"));
        return false;
    }
    checkVersion();
    return true;
}

bool Client::connectToServer() {
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    console::out::inf(
        lang::gt("client_class.connect_to_server", config_values.server_address, config_values.server_port));
    boost::system::error_code ec{};
    const auto endpoints{resolver.resolve(config_values.server_address, std::to_string(config_values.server_port), ec)};
    if (!checkErr(ec)) {
        return false;
    }
    boost::asio::connect(socket, endpoints, ec);
    if (!checkErr(ec)) {
        return false;
    }
    console::out::inf(lang::gt("client_class.successfull_connection"));
    return true;
}

bool Client::checkWelcomeMessage() {
    console::out::verbose(lang::gt("client_class.send_welcome_message"));
    boost::asio::write(socket, boost::asio::buffer(WELCOME_MESSAGE));
    bool response{};
    if (!network::readInt<bool>(socket, response)) {
        return false;
    }
    if (!response) {
        console::out::err(lang::gt("network.error.invalid_welcome_msg"));
        return false;
    }
    return true;
}

void Client::checkVersion() {
    const pds::version_t version{constants::VERSION.combine()};
    console::out::verbose(lang::gt("client_class.send_version", constants::VERSION.toString(), version));
    if (!network::sendInt<pds::version_t>(socket, version)) {
        console::out::err(lang::gt("client_class.error.send_version"));
        return;
    }
    if (!readVersion(server_ver) || !readVersion(min_client_version) || !readVersion(max_client_version)) {
        return;
    }
    console::out::verbose(lang::gt("client_class.server_version", server_ver.toString(), server_ver.combine()));
    printVersionsMessage();
}

bool Client::readVersion(Version& ver) {
    pds::version_t ver_int{};
    if (!network::readInt<pds::version_t>(socket, ver_int)) {
        console::out::err(lang::gt("client_class.error.read_version"));
        return false;
    }
    ver.setFromCombined(ver_int);
    return true;
}

void Client::printVersionsMessage() {
    const pds::version_t ver_combined{constants::VERSION.combine()};
    if (ver_combined > max_client_version.combine()) {
        console::out::err(lang::gt("client_class.warning.server_too_old"));
    } else if (ver_combined < min_client_version.combine()) {
        console::out::err(lang::gt("client_class.warning.client_too_old"));
    } else {
        console::out::verbose(lang::gt("client_class.version_ok"));
    }
}

bool Client::checkErr(const boost::system::error_code& ec) {
    if (!ec) {
        return true;
    }
    console::out::err(lang::gt("client_class.error.connect_to_server", ec.message()));
    return false;
}

void Client::close() {
    resetState();
    const std::vector<char> QUIT_BUFFER{static_cast<pds::request_id_t>(RequestId::Quit), 0};
    network::sendBuffer(socket, QUIT_BUFFER);
    socket.close();
}

tcp::socket& Client::getSocket() {
    return socket;
}

User& Client::getUser() {
    return user;
}

const User& Client::getUser() const {
    return user;
}

bool Client::isLogged() const {
    return is_logged;
}

bool Client::isConnected() const {
    return is_connected;
}

void Client::setUser(const User& user) {
    this->user = user;
}

void Client::setLogged(bool is_logged) {
    this->is_logged = is_logged;
}
