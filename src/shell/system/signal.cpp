#include "shell/system/signal.hpp"

#include <csignal>
#include <cstdlib>
#include <string>

#include "Shared/io/console.hpp"
#include "network/client.hpp"
#include "shell.hpp"

namespace {

inline Shell* shell_{};

void chillHandle(int signal) {
    console::out::inf("signal triggered (" + std::to_string(signal) + "), exiting...");
    Client& client{shell_->getClient()};
    client.close();
    std::exit(signal);
}

void criticalHandle(int signal) {
    console::out::err("critical error ! signal: " + std::to_string(signal));
    std::exit(signal);
}

} // namespace

void shell::signal::setSignals(Shell* shell) {
    ::shell_ = shell;

    std::signal(SIGINT, chillHandle);
    std::signal(SIGTERM, chillHandle);
    std::signal(SIGBREAK, chillHandle);

    std::signal(SIGABRT, criticalHandle);
    std::signal(SIGFPE, criticalHandle);
    std::signal(SIGILL, criticalHandle);
    std::signal(SIGSEGV, criticalHandle);
}
