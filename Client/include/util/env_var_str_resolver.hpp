#pragma once

#include <string>

class EnvVarStrResolver final {
  private:
    std::string input{}, output{};
    bool is_in_var{};
    std::string current_var{};

    void processNextChar(char c);
    void insertEnvVar();

  public:
    EnvVarStrResolver() = default;

    void resolve();

    void setInput(const std::string& input);
    const std::string& getOutput() const;
};
