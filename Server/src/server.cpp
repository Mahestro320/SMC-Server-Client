#include "server.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/socket_base.hpp>
#include <boost/system/detail/error_code.hpp>
#include <cstdint>
#include <exception>
#include <stop_token>
#include <string>
#include <thread>
#include <utility>

#include "Shared/io/console.hpp"
#include "Shared/pds.hpp"
#include "common.hpp"
#include "io/logger.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "server/config.hpp"
#include "users/manager.hpp"
#include "util/thread.hpp"

using boost::asio::ip::tcp;

namespace {

thread_local uint32_t thread_index{UINT32_MAX};
boost::asio::io_context io_context{};
std::atomic<pds::client_count_t> client_count{};

void init() {
    common::main_thread_id = util::thread::getCurrentThreadId();
    Config& config{Config::getInstance()};
    config.load();
    Logger& logger{Logger::getInstance()};
    logger.init();
    console::init();
    UsersManager& users_manager{UsersManager::getInstance()};
    users_manager.load();
}

void handleClient(tcp::socket socket) {
    try {
        const pds::client_count_t client_idx{client_count.load(std::memory_order_acquire)};
        client_count.fetch_add(1, std::memory_order_release);

        Client::setInstance(std::move(socket), client_idx);
        Client& client{Client::getInstance()};
        client.start();
    } catch (const std::bad_alloc& e) {
        console::out::err("memory error: " + std::string{e.what()});
    } catch (const std::exception& e) {
        console::out::err("unhandled exception: " + std::string{e.what()});
    }
    console::out::inf("thread stopped");
}

void acceptConnections(tcp::acceptor& acceptor, boost::asio::io_context& io_context) {
    acceptor.async_accept([&](const boost::system::error_code& ec, tcp::socket socket) {
        if (ec) {
            console::out::err("error while accepting connection: " + ec.message());
        } else {
            std::thread t{[socket = std::move(socket)]() mutable { handleClient(std::move(socket)); }};
            t.detach();
        }
        acceptConnections(acceptor, io_context);
    });
}

void runServer(pds::connection_port_t port) {
    tcp::endpoint endpoint{tcp::v4(), port};
    tcp::acceptor acceptor{io_context};

    acceptor.open(endpoint.protocol());
    acceptor.set_option(boost::asio::socket_base::reuse_address{true});
    acceptor.bind(endpoint);
    acceptor.listen();

    console::out::inf("server started, waiting for connections");
    acceptConnections(acceptor, io_context);
    io_context.run();
}

} // namespace

void server::run() {
    ::init();
    Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    console::out::inf("starting server <:color=green>v" + common::VERSION.toString() +
                      "<:color=reset> (<:color=bright_blue>" + network::getIpv4Address() + ':' +
                      std::to_string(config_values.server_port) + "<:color=reset>)");
    console::out::inf("time format: <:color=bright_blue>" + config_values.time_format);
    ::runServer(config_values.server_port);
}
