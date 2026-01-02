#include <string>
#include <vector>
#include <limits>
#include <type_traits>

#include "Shared/io/console.hpp"
#include "Shared/util/concepts.hpp"

template<is_integral T> bool ArrayAnalyzer::extractInt(T& output) {
    if (data.size() - cursor < sizeof(T)) {
        setSizeError("int", sizeof(T));
        return false;
    }
    using U = std::make_unsigned_t<T>;
    U value{};
    for (size_t i{}; i < sizeof(T); ++i) {
        value |= static_cast<U>(static_cast<unsigned char>(data[cursor + i])) << (i * 8);
    }
    output = static_cast<T>(value);
    cursor += sizeof(T);
    return true;
}

template<is_integral StringSizeType> bool ArrayAnalyzer::extractString(std::string& output) {
    std::vector<char> buffer{};
    if (!extractBuffer<StringSizeType>(buffer)) {
        return false;
    }
    output = std::string{buffer.begin(), buffer.end()};
    return true;
}

template<is_integral BufferSizeType> bool ArrayAnalyzer::extractBuffer(std::vector<char>& output) {
    BufferSizeType size{};
    if (!extractInt<BufferSizeType>(size)) {
        return false;
    }
    if (data.size() - cursor < size) {
        setSizeError("buffer", size);
        return false;
    }
    output.resize(static_cast<size_t>(size));
    for (BufferSizeType i{}; i < size; i++) {
        output[i] = data[cursor + i];
    }
    cursor += size;
    return true;
}

template<is_integral T> void ArrayBuilder::insertInt(T value) {
    const size_t cursor{data.size()};
    data.resize(cursor + sizeof(T));
    for (size_t i{}; i < sizeof(T); i++) {
        data[cursor + i] = static_cast<char>((value >> (i * 8)) & 0xFF);
    }
}

template<is_integral StringSizeType> void ArrayBuilder::insertString(const std::string& str) {
    insertBuffer<StringSizeType>(std::vector<char>{str.begin(), str.end()});
}

template<is_integral BufferSizeType> void ArrayBuilder::insertBuffer(const std::vector<char>& buffer) {
    size_t buffer_size{buffer.size()};
    constexpr size_t max_size{std::numeric_limits<BufferSizeType>::max()};
    if (buffer_size > max_size) {
        console::out::err("array builder: type value range is too small to contain number, truncating value");
        buffer_size = max_size;
    }
    insertInt<BufferSizeType>(static_cast<BufferSizeType>(buffer_size));
    const size_t cursor{data.size()};
    data.resize(cursor + buffer_size);
    std::memcpy(data.data() + cursor, buffer.data(), buffer_size);
}
