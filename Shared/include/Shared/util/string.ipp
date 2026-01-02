#include "Shared/io/console.hpp"

#include <exception>
#include <string>

#include "concepts.hpp"

template<is_arithmetic T> bool util::string::stoi(const std::string& str, T& out) {
    try {
        out = static_cast<T>(std::stoi(str));
    } catch (const std::exception& e) {
        console::out::err("stoi exception: " + std::string{e.what()});
        return false;
    }
    return true;
}
