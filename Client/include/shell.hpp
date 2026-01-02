#pragma once

#include <memory>
#include <string>
#include <vector>

#include "shell/command.hpp"
#include "shell/exit_code.hpp"

class Client;
struct CommandInfos;

class Shell final {
  private:
    Client& client;
    float command_time{};

    std::vector<std::string> getCommandTokens();
    exit_code_t runCommand(const std::vector<std::string>& command_tokens);
    std::vector<std::string> getArgsFromTokens(std::vector<std::string> tokens) const;
    bool canExecuteCmdName(const std::string& name) const;
    bool canExecuteCmdInstance(const CommandInfos& cmd) const;
    std::unique_ptr<Command> getCommandInstanceFromName(const std::string& name) const;
    std::string getCommandInputStartInfo() const;
    void printFinalCmdMessage(exit_code_t code) const;

  public:
    Shell(Client& client);

    void start();
    void processNewCommand();
    Client& getClient();
};
