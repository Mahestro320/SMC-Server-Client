#include "util/thread.hpp"

#include <cstdint>
#include <thread>

uint32_t util::thread::getCurrentThreadId() {
    std::hash<std::thread::id> hasher{};
    return static_cast<uint32_t>(hasher(std::this_thread::get_id()));
}
