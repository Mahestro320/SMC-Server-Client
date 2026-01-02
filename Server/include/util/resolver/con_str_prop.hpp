#pragma once

/*
 * Console String Property Resolver
 *
 * resolves the values between <>
 * for example:
 *
 * "Hello <:color=yellow>everyone<:color=reset> !"
 *
 * "everyone" will be yellow.
 */

#include <iostream>
#include <string>

class ConStrPropResolver final {
  private:
    std::string text{};
    std::ostream* stream{&std::cout};

    std::string current_prop{};
    bool is_in_prop{};

    void processChar(char c);
    void processRawProp();
    void processProp();
    void processProp(const std::string& key, const std::string& val);
    void setColor(const std::string& color);

  public:
    ConStrPropResolver() = default;

    void setText(const std::string& text);
    void setStream(std::ostream* stream);

    void print();
};
