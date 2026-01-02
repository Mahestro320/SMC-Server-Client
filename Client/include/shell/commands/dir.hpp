#pragma once

#include <cstdint>
#include <vector>

#include "../command.hpp"
#include "../exit_code.hpp"
#include "Shared/io/file_info.hpp"

class DirCommand final : public Command {
  private:
    static inline constexpr const char VERTICAL_BAR[]{" | "};
    static inline constexpr const char SPACE{' '};
    static inline constexpr const char BEGIN_SPACES[]{"   "};
    static inline constexpr const char NEW_LINE{'\n'};

    static inline constexpr uint64_t TYPE_SPACES_COUNT{10};
    static inline constexpr uint64_t SIZE_SPACES_COUNT{10};

    std::vector<FileInfo> data{};

    void sortData();
    void printData() const;
    void printArrayHeader() const;
    void printArrayContent() const;

  public:
    DirCommand() = default;

    exit_code_t run() override;
};
