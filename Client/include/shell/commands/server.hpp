#pragma once

#include "../command.hpp"
#include "../exit_code.hpp"

#include "Shared/server/restart_modes.hpp"

class ServerCommand : public Command {
  private:
    static inline constexpr const char OPTION_GET_VERSION[]{"get-version"};
    static inline constexpr const char OPTION_RESTART[]{"restart"};
    static inline constexpr const char OPTION_RESTART_PROGRAM[]{"program"};
    static inline constexpr const char OPTION_RESTART_MACHINE[]{"machine"};

    bool runOption(const std::string& option);

    bool runGetVersionOption();
    bool runRestartSubOption();

    bool restartServer(RestartMode mode);
    bool canRestart() const;

  public:
    ServerCommand() = default;

    exit_code_t run() override;
};
