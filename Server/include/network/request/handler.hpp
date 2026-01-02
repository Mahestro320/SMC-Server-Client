#pragma once

#include <span>

#include "Shared/user/role.hpp"
#include "Shared/util/version.hpp"
#include "network/request/analyzer.hpp"
#include "network/response/builder.hpp"

class Client;

struct RHRequirements {
    Version min_version;
    bool must_be_logged{false};
    Role min_role{Role::User};
};

// Request handler
class RH {
  private:
    RHRequirements requirements{};
    bool response_sent{};

  protected:
    Client* client{};
    RequestAnalyzer request{};
    ResponseBuilder response{};

    virtual void _run() = 0;

    bool checkVersion();
    bool checkMustBeLogged();
    bool checkMinRole();

    // this function can be executed just once.
    // if it's not called in herited classes, it will be called automatically at the end of run()
    bool sendResponse();

  public:
    RH(const RHRequirements& req);

    void setClient(Client* client);
    void setRequestContent(const std::span<char>& content);
    void run();
};
