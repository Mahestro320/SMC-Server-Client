#include "Shared/io/console.hpp"

#include <iostream>
#include <string>

std::string console::in::getLine() {
    std::string line{};
    std::getline(std::cin, line);
    return line;
}
