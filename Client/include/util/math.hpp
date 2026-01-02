#pragma once

namespace util::math {

template<typename T> extern T minusNotNul(T a, T b) {
    return (b > a) ? T{} : a - b;
}

} // namespace util::math
