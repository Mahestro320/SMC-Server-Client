#pragma once

#include <string>

#include "../command.hpp"
#include "../exit_code.hpp"

class LangCommand final : public Command {
  private:
    static inline constexpr char OPTION_INFO[]{"info"};
    static inline constexpr char OPTION_RELOAD[]{"reload"};
    static inline constexpr char OPTION_SET[]{"set"};

    static inline constexpr char SMCL_EXTENSION[]{"smcl"};

    bool runOption(const std::string& option);

    bool runOptionInfo();
    bool runOptionReload();
    bool runOptionSet();

    bool tryToSetWithFullFileName(const std::string& arg);
    bool tryToSetWithFileName(const std::string& arg);

  public:
    LangCommand() = default;

    exit_code_t run() override;
};
