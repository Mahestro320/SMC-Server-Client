#include "network.hpp"

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/basic_resolver.hpp>
#include <boost/system/detail/error_code.hpp>
#include <string>
#include <vector>

#include "Shared/io/console.hpp"
#include "Shared/pds.hpp"
#include "network.hpp"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

namespace {

void pinf(const std::string& msg, bool silent) {
    if (!silent) {
        console::out::inf(msg);
    }
}

} // namespace

bool network::sendBuffer(tcp::socket& socket, const std::vector<char>& buffer, bool silent) {
    const pds::network_buffer_t buffer_size{static_cast<pds::network_buffer_t>(buffer.size())};
    pinf("sending buffer size (<:color=bright_blue>" + std::to_string(buffer_size) + "<:color=reset>)", silent);
    if (!sendInt<pds::network_buffer_t>(socket, buffer_size)) {
        return false;
    }
    pinf("sending buffer content", silent);
    boost::system::error_code error_code{};
    const size_t bytes_write{boost::asio::write(socket, boost::asio::buffer(buffer), error_code)};
    if (error_code) {
        console::out::err("error while sending buffer content: " + error_code.message());
        return false;
    }
    if (bytes_write < buffer_size) {
        console::out::err("the buffer isn't fully sent !");
        return false;
    }
    return true;
}

bool network::sendString(tcp::socket& socket, const std::string& str, bool silent) {
    pinf("sending string", silent);
    return sendBuffer(socket, std::vector<char>(str.begin(), str.end()));
}

bool network::readBuffer(tcp::socket& socket, std::vector<char>& buffer, bool silent) {
    pinf("reading buffer size", silent);
    boost::system::error_code error_code{};
    pds::network_buffer_t buffer_size{};
    if (!readInt<pds::network_buffer_t>(socket, buffer_size)) {
        return false;
    }
    buffer.resize(buffer_size);
    pinf("reading buffer content", silent);
    const size_t bytes_read{boost::asio::read(socket, boost::asio::buffer(buffer, buffer_size), error_code)};
    if (error_code) {
        console::out::err(error_code.message());
        return false;
    }
    if (bytes_read < buffer_size) {
        console::out::err("the buffer isn't fully readed !");
        return false;
    }
    return true;
}

bool network::readString(boost::asio::ip::tcp::socket& socket, std::string& str, bool silent) {
    pinf("reading string", silent);
    std::vector<char> data(str.begin(), str.end());
    if (!readBuffer(socket, data)) {
        return false;
    }
    str = std::string(data.begin(), data.end());
    pinf("string content: <:color=green>\"" + str + "\"<:color=reset>", silent);
    return true;
}

std::string network::getIpv4Address() {
    boost::asio::io_context io_ctx{};
    std::string hostname{boost::asio::ip::host_name()};
    tcp::resolver resolver{io_ctx};
    boost::system::error_code error_code{};
    tcp::resolver::results_type results{resolver.resolve(hostname, "", error_code)};
    if (error_code) {
        return "unknown";
    }
    for (const auto& entry : results) {
        auto endpoint{entry.endpoint()};
        boost::asio::ip::address address{endpoint.address()};
        if (address.is_v4() && !address.is_loopback()) {
            return address.to_string();
        }
    }
    return "unknown";
}
