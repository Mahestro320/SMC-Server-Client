#pragma once

#include <string>

#include "../command.hpp"
#include "../exit_code.hpp"

class ClientCommand final : public Command {
  private:
    static inline constexpr const char OPTION_CONNECT[]{"connect"};
    static inline constexpr const char OPTION_DISCONNECT[]{"disconnect"};

    bool runOption(const std::string& option);

    bool runConnectOption();
    bool runDisconnectOption();

  public:
    ClientCommand() = default;

    exit_code_t run() override;
};
