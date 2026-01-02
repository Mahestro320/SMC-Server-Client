#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "Shared/pds.hpp"
#include "Shared/util/version.hpp"
#include "util/resolver/evs_str.hpp"

struct ConfigValues {
    pds::connection_port_t server_port{};

    std::string dirs_storage{};
    std::string dirs_logs{};

    Version client_min_version{};
    Version client_max_version{};

    std::string time_format{};

    pds::network_buffer_t io_load_max_buffer_size{};
};

class Config final {
  private:
    boost::property_tree::ptree property_tree{};
    ConfigValues values{};

    bool loadIniFile();
    bool tryToLoadValuesFromTree();
    void loadValuesFromTree();
    void setDirsValues() const;

    template<typename T> T getValueFromTree(const std::string& key) {
        if constexpr (std::is_same_v<T, std::string>) {
            const std::string raw_val{property_tree.get<std::string>(key)};
            EVSStrResolver<> resolver{};
            resolver.setInput(raw_val);
            if (!resolver.resolve()) {
                throw std::runtime_error{"error while resolving key " + key};
            }
            return resolver.getOutput();
        } else {
            return property_tree.get<T>(key);
        }
    }

    Config() = default;

  public:
    static Config& getInstance();

    bool load();

    const ConfigValues& getValues() const;
};
