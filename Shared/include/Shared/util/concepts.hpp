#pragma once

#include <type_traits>

template<typename Derived, typename Base>
concept is_base_of = std::is_base_of_v<Base, Derived>;

template<typename T>
concept is_integral = std::is_integral_v<T> && !std::is_same_v<T, bool>;

template<typename T>
concept is_arithmetic = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

template<typename T>
concept int_or_string = std::is_same_v<T, int> || std::is_same_v<T, std::string>;
