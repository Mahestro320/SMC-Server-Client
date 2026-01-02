#pragma once

#include <concepts>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

template<typename T>
concept FormatType = std::is_arithmetic_v<std::decay_t<T>> || std::same_as<std::decay_t<T>, bool> ||
                     std::same_as<std::decay_t<T>, char> || std::same_as<std::decay_t<T>, std::string> ||
                     std::same_as<std::decay_t<T>, const char*>;

class SMCLFormatter final {
  private:
    static constexpr size_t ARG_LENGTH{3}; // ${}

    const std::string& input;
    std::string output{};
    std::vector<std::string> args_str{};

    size_t arg_idx{};
    size_t curr_idx{};

    template<FormatType... Args> void buildArgsStr(Args&&... args) {
        (args_str.push_back(toString(std::forward<Args>(args))), ...);
    }

    template<FormatType T> std::string toString(T&& value) {
        if constexpr (std::is_same_v<std::decay_t<T>, bool>) {
            return value ? "true" : "false";
        } else if constexpr (std::is_arithmetic_v<std::decay_t<T>>) {
            return std::to_string(value);
        } else if constexpr (std::is_same_v<std::decay_t<T>, std::string>) {
            return value;
        } else if constexpr (std::is_same_v<std::decay_t<T>, const char*> || std::is_same_v<std::decay_t<T>, char>) {
            return std::string{value};
        }
        std::ostringstream oss{};
        oss << value;
        return oss.str();
    }

    void processChar();
    bool isArgHere() const;

  public:
    SMCLFormatter(const std::string& str);

    template<FormatType... Args> void format(Args&&... args) {
        buildArgsStr(std::forward<Args>(args)...);
        for (; curr_idx < input.size(); curr_idx++) {
            processChar();
        }
    }

    const std::string& getOutput() const;
};
