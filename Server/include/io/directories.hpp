#pragma once

#include <filesystem>

namespace dirs {

constexpr const char* COMPILATION_CURR_FILE_STR{__FILE__};
inline const auto COMPILATION_CURR_FILE{std::filesystem::path{COMPILATION_CURR_FILE_STR}};
inline const auto COMPILATION_ROOT{COMPILATION_CURR_FILE.parent_path().parent_path().parent_path()};

inline const auto ROOT{std::filesystem::current_path()};
inline const auto DATA{ROOT / "data"};
inline const auto D_CONFIG{DATA / "config.ini"};

inline std::filesystem::path logs{};
inline std::filesystem::path strg{}; // storage

inline std::filesystem::path s_users{};
inline std::filesystem::path s_network{};
inline std::filesystem::path s_network_shared{};

extern void setLogs(const std::filesystem::path& path);
extern void setStorage(const std::filesystem::path& path);

}; // namespace dirs
