#pragma once

#include <cstdint>
#include <string>

#include "../command.hpp"
#include "../exit_code.hpp"

struct CommandInfos;

class HelpCommand final : public Command {
  private:
    static inline constexpr const char VERTICAL_BAR[]{" | "};
    static inline constexpr const char BEGIN_SPACES[]{"   "};

    static inline constexpr uint64_t NAME_SPACES_COUNT{15};
    static inline constexpr uint64_t SHORT_NAME_SPACES_COUNT{15};
    static inline constexpr uint64_t SHORT_DESCR_SPACES_COUNT{50};
    static inline constexpr uint64_t CAN_USE_OFFLINE_SPACES_COUNT{20};

    bool canSee(const CommandInfos& cmd) const;
    void printAllCommandsInfos() const;
    void printBeginMessage() const;
    void printCommandInfos(const std::string& cmd_name, const CommandInfos& cmd) const;
    exit_code_t printLongDescription() const;
    void printLongDescriptionOf(const std::string& cmd_name, const CommandInfos& cmd) const;

  public:
    HelpCommand() = default;

    exit_code_t run() override;
};
