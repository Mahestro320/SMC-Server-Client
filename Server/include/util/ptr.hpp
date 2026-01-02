#pragma once

namespace util::ptr {

template<typename T> void copyIfNotNullptr(T* var, const T& ptr) {
    if (var) {
        *var = ptr;
    }
}

} // namespace util::ptr

#include "ptr.ipp"
