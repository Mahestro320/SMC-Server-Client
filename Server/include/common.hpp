#pragma once

#include <atomic>
#include <cstdint>

#include "Shared/util/version.hpp"

namespace common {

inline constexpr char UNDEFINED_TEXT[]{"undefined"};
inline constexpr Version VERSION{1, 0, 0};

inline std::atomic<bool> shutdown_requested{};
inline uint32_t main_thread_id{};

} // namespace common
