#pragma once

#include <filesystem>
#include <vector>

#include "network/with_client.hpp"

class GetDirContentRecursively final : public WithClient {
  private:
    std::filesystem::path input_path{};
    std::vector<std::filesystem::path> content{};

    bool addContentOf(const std::filesystem::path& path);

  public:
    bool run();

    void setPath(const std::filesystem::path& path);
    const std::vector<std::filesystem::path>& getContent() const;
    std::vector<std::filesystem::path>&& getContent();
};
