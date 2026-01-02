#pragma once

#include <string>

#include "../command.hpp"
#include "../exit_code.hpp"

class ConfCommand final : public Command {
  private:
    static inline constexpr const char OPTION_LOAD[]{"load"};
    static inline constexpr const char OPTION_SAVE[]{"save"};
    static inline constexpr const char OPTION_PRINT[]{"print"};
    static inline constexpr const char OPTION_GET[]{"get"};
    static inline constexpr const char OPTION_SET[]{"set"};

    bool runOption(const std::string& name);
    bool runLoadOption() const;
    bool runSaveOption() const;
    bool runPrintOption() const;
    bool runSetOption() const;
    bool runGetOption() const;

  public:
    ConfCommand() = default;

    exit_code_t run() override;
};
