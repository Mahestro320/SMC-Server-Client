#include "io/directories.hpp"

#include <filesystem>

namespace fs = std::filesystem;

namespace {

void updatePaths() {
    dirs::s_users = dirs::strg / "users";
    dirs::s_network = dirs::strg / "network";
    dirs::s_network_shared = dirs::s_network / "shared";
}

} // namespace

void dirs::setLogs(const fs::path& path) {
    logs = path;
}

void dirs::setStorage(const fs::path& path) {
    strg = path;
    updatePaths();
}
