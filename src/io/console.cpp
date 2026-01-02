#include "Shared/io/console.hpp"

#include <Windows.h>
#include <exception>
#include <iostream>
#include <mutex>
#include <string>

#include "shell/config.hpp"

namespace {

std::mutex mtx{};

}

void console::init() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void console::out::inf(const std::string& msg, bool line_return) {
    std::lock_guard lock{mtx};
    std::cout << msg << ((line_return) ? "\n" : "");
}

void console::out::err(const std::string& msg, bool line_return) {
    std::lock_guard lock{mtx};
    std::cerr << msg << ((line_return) ? "\n" : "");
}

void console::out::err(const std::exception& e, bool line_return) {
    console::out::err(e.what());
}

void console::out::verbose(const std::string& msg, bool line_return) {
    Config& config{Config::getInstance()};
    ConfigValues config_values{config.getValues()};
    if (config_values.shell_verbose) {
        console::out::inf(msg, line_return);
    }
}
