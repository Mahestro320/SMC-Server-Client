#pragma once

template<typename T> struct Vector2 final {
    T x{}, y{};
};

using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;
using Vector2f = Vector2<float>;
