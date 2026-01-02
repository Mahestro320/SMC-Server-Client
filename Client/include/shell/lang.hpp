#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Shared/util/version.hpp"
#include "shell/lang/smcl_formatter.hpp"

struct LangData final {
    std::unordered_map<std::string, std::string> translations{};

    std::string name{};
    std::string locale{};
    std::vector<std::string> authors{};
    Version version{};
};

namespace lang {

static const std::string UNKNOWN{"???"};

extern bool loadFile();
extern bool exists(const std::string& key);

extern const std::string& getName();
extern const std::string& getLocale();
extern const std::vector<std::string>& getAuthors();
extern const Version& getVersion();

extern const std::string& gt(const std::string& key);

template<typename... Args> extern std::string gt(const std::string& key, Args&&... args) {
    const std::string& str{gt(key)};
    SMCLFormatter formatter{str};
    formatter.format(std::forward<Args>(args)...);
    return formatter.getOutput();
}

} // namespace lang
