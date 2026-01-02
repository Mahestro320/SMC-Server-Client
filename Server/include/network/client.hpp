#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <span>
#include <string>

#include "Shared/network/request/id.hpp"
#include "Shared/network/response/error.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/version.hpp"
#include "io/sfs.hpp"
#include "users/user.hpp"

class RH;

class Client final {
  private:
    static inline constexpr size_t WELCOME_MESSAGE_SIZE{3};
    static inline constexpr char WELCOME_MESSAGE[WELCOME_MESSAGE_SIZE]{'S', 'M', 'C'};

    static thread_local std::unique_ptr<Client> instance;

    boost::asio::ip::tcp::socket socket;
    pds::client_count_t index{};
    Version version{};

    bool is_connected{true};
    bool is_logged{false};
    User user{};
    SFS sfs{*this};

    boost::asio::ip::tcp::endpoint getRemoteEndpoint() const;
    void printLocation() const;
    void startCommunication();
    bool checkWelcomeMessage();
    bool isWelcomeMessage(const std::string& message) const;
    void checkVersion();
    void handleNewRequest();
    void processReceivedRequest(RequestId request, const std::span<char>& request_content);
    std::unique_ptr<RH> getHandlerFromRequest(RequestId id) const;
    void sendResponseError(ResponseError err);

  public:
    Client(boost::asio::ip::tcp::socket socket, pds::client_count_t index);
    Client() = delete;

    static void setInstance(boost::asio::ip::tcp::socket socket, pds::client_count_t index);
    static Client& getInstance();

    void start();

    bool isLogged() const;
    boost::asio::ip::tcp::socket& getSocket();
    const User& getUser() const;
    SFS& getSFS();
    pds::client_count_t getIndex() const;
    pds::connection_port_t getPort() const;
    std::string getAddress() const;
    const Version& getVersion() const;

    void setUser(const User& user);
    void setLogged(bool is_logged);
};
