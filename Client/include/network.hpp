#pragma once

#include <boost/asio.hpp>
#include <boost/system/detail/error_code.hpp>
#include <concepts>
#include <string>
#include <vector>

#include "Shared/io/console.hpp"
#include "boost/asio/ip/tcp.hpp"
#include "shell/lang.hpp"

class Client;

namespace network {

extern bool sendString(boost::asio::ip::tcp::socket& socket, const std::string& str);
extern bool sendBuffer(boost::asio::ip::tcp::socket& socket, const std::vector<char>& buffer);

extern bool readString(boost::asio::ip::tcp::socket& socket, std::string& str);
extern bool readBuffer(boost::asio::ip::tcp::socket& socket, std::vector<char>& buffer);

template<std::integral T> bool sendInt(boost::asio::ip::tcp::socket& socket, T data) {
    boost::system::error_code error_code{};
    boost::asio::write(socket, boost::asio::buffer(&data, sizeof(T)), error_code);
    if (error_code) {
        console::out::err(lang::gt("network.error.send_int", error_code.message()));
        return false;
    }
    return true;
}

template<std::integral T> bool readInt(boost::asio::ip::tcp::socket& socket, T& data) {
    boost::system::error_code error_code{};
    const size_t bytes_read{boost::asio::read(socket, boost::asio::buffer(&data, sizeof(T)), error_code)};
    if (error_code) {
        console::out::err(lang::gt("network.error.read_int", error_code.message()));
        return false;
    }
    if (bytes_read < sizeof(T)) {
        console::out::err(lang::gt("network.error.int_not_read_completely"));
        return false;
    }
    return true;
}

} // namespace network
