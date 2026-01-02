#pragma once

#include <filesystem>

namespace dirs {

inline const auto ROOT{std::filesystem::current_path()};
inline const auto DATA{ROOT / "data"};
inline const auto D_CONFIG{DATA / "config.ini"};
inline const auto D_LANG{DATA / "lang"};

}; // namespace dirs
