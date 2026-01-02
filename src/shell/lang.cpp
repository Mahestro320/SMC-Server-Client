#include "shell/lang.hpp"

#include <string>
#include <vector>

#include "Shared/util/version.hpp"
#include "io/directories.hpp"
#include "shell/config.hpp"
#include "shell/lang/smcl_loader.hpp"

namespace {

LangData data{};

} // namespace

bool lang::loadFile() {
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    const auto file{dirs::D_LANG / (config_values.shell_language + ".smcl")};
    SMCLLoader loader{};
    loader.setData(&data);
    if (!loader.load(file)) {
        return false;
    }
    return true;
}

bool lang::exists(const std::string& key) {
    return data.translations.contains(key);
}

const std::string& lang::getName() {
    return data.name;
}

const std::string& lang::getLocale() {
    return data.locale;
}

const std::vector<std::string>& lang::getAuthors() {
    return data.authors;
}

const Version& lang::getVersion() {
    return data.version;
}

const std::string& lang::gt(const std::string& key) {
    if (exists(key)) {
        return data.translations.at(key);
    }
    return UNKNOWN;
}
