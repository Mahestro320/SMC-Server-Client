#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "Shared/util/concepts.hpp"

class ArrayEditor {
  protected:
    std::vector<char> data{};

    ArrayEditor() = default;

  public:
    virtual ~ArrayEditor() = default;
};

class ArrayAnalyzer : public ArrayEditor {
  private:
    void setSizeError(const std::string& extract_target, size_t size_required);

  protected:
    size_t cursor{};
    std::string last_error{};

  public:
    ArrayAnalyzer() = default;

    void setCursor(size_t cursor);
    void setData(std::vector<char>&& data);
    void clearError();

    const std::string& getLastError() const;
    size_t getSize() const;
    bool isError() const;

    bool extractBool(bool& output);
    bool extractPath(std::filesystem::path& path);

    template<is_integral T> bool extractInt(T& output);
    template<is_integral StringSizeType> bool extractString(std::string& output);
    template<is_integral BufferSizeType> bool extractBuffer(std::vector<char>& output);
};

class ArrayBuilder : public ArrayEditor {
  public:
    ArrayBuilder() = default;

    const std::vector<char>& getResult() const;

    void insertBool(bool value);
    void insertPath(const std::filesystem::path& path);

    template<is_integral T> void insertInt(T value);
    template<is_integral StringSizeType = uint32_t> void insertString(const std::string& str);
    template<is_integral BufferSizeType = uint32_t> void insertBuffer(const std::vector<char>& buffer);
};

#include "array_editor.ipp"
