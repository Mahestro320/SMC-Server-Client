#pragma once

#include <filesystem>

#include "Shared/network/array_editor.hpp"
#include "network/client.hpp"
#include "network/response/builder.hpp"

class RequestAnalyzer final : public ArrayAnalyzer {
  public:
    RequestAnalyzer() = default;

    bool extractAbsPath(Client& client, std::filesystem::path& output, ResponseBuilder& response);
};
