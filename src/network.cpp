#include "network.hpp"

#include <boost/asio.hpp>
#include <boost/system/detail/error_code.hpp>
#include <string>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/pds.hpp"
#include "shell/lang.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

bool network::sendString(tcp::socket& socket, const std::string& str) {
    return sendBuffer(socket, std::vector<char>(str.begin(), str.end()));
}

bool network::sendBuffer(tcp::socket& socket, const std::vector<char>& buffer) {
    const pds::network_buffer_t buffer_size{static_cast<pds::network_buffer_t>(buffer.size())};
    if (!sendInt<pds::network_buffer_t>(socket, buffer_size)) {
        return false;
    }
    boost::system::error_code error_code{};
    const size_t bytes_write{boost::asio::write(socket, boost::asio::buffer(buffer), error_code)};
    if (error_code) {
        console::out::err(lang::gt("network.error.send_buffer_content", error_code.message()));
        return false;
    }
    if (bytes_write < buffer_size) {
        console::out::err(lang::gt("network.error.buffer_not_sent_completely"));
        return false;
    }
    return true;
}

bool network::readString(tcp::socket& socket, std::string& str) {
    std::vector<char> data(str.begin(), str.end());
    if (!readBuffer(socket, data)) {
        return false;
    }
    str = std::string(data.begin(), data.end());
    return true;
}

bool network::readBuffer(tcp::socket& socket, std::vector<char>& buffer) {
    boost::system::error_code error_code{};
    pds::network_buffer_t buffer_size{};
    if (!readInt<pds::network_buffer_t>(socket, buffer_size)) {
        return false;
    }
    buffer.resize(static_cast<size_t>(buffer_size));
    const size_t bytes_read{boost::asio::read(socket, boost::asio::buffer(buffer, buffer_size), error_code)};
    if (error_code) {
        console::out::err(lang::gt("network.error.no_message", error_code.message()));
        return false;
    }
    if (bytes_read < buffer_size) {
        console::out::err(lang::gt("network.error.buffer_not_read_completely"));
        return false;
    }
    return true;
}
