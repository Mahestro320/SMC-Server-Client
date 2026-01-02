#include "network/request/analyzer.hpp"

#include <filesystem>

#include "io/sfs.hpp"
#include "network/client.hpp"
#include "network/response/builder.hpp"

bool RequestAnalyzer::extractAbsPath(Client& client, std::filesystem::path& output, ResponseBuilder& response) {
    std::filesystem::path path_not_abs{};
    if (!extractPath(path_not_abs)) {
        response.buildErrorFromInvalidRequest(*this);
        return false;
    }
    const SFS& sfs{client.getSFS()};
    output = sfs.absoluteFromUserScope(path_not_abs);
    return true;
}
