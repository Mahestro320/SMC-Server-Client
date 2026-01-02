#pragma once

#include <filesystem>

#include "../handler.hpp"
#include "Shared/pds.hpp"

class IODeleteFileRH : public RH {
  private:
    std::filesystem::path path{};
    pds::file_count_t elems_removed{};

    bool _run() override;

  public:
    IODeleteFileRH();

    void setFilePath(const std::filesystem::path& path);
    pds::file_count_t getElemsRemoved() const;
};
