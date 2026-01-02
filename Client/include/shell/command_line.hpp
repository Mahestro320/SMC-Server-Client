#pragma once

#include <string>
#include <vector>

class CommandLine {
  private:
    std::string raw_line{};
    std::vector<std::string> tokenized_line{};

    std::string current_item{};
    bool is_in_simple_quotes{};
    bool is_in_double_quotes{};
    char last{};

    bool processSimpleQuotes(char c);
    bool processDoubleQuotes(char c);

    void flushToken(std::string& item);

  public:
    CommandLine() = default;

    bool getLine();
    void tokenize();
    bool empty() const;
    const std::vector<std::string>& getTokens() const;
};
