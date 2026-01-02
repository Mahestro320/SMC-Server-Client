#include "shell/config.hpp"

#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <cstdint>
#include <string>
#include <type_traits>

#include "Shared/io/console.hpp"
#include "common.hpp"
#include "io/directories.hpp"
#include "util/env_var_str_resolver.hpp"

using boost::property_tree::ptree;

Config& Config::getInstance() {
    static Config instance{};
    return instance;
}

bool Config::load() {
    return loadIniFile() && tryToLoadValuesFromTree();
}

bool Config::loadIniFile() {
    console::out::inf("[config] loading file");
    try {
        boost::property_tree::ini_parser::read_ini(dirs::D_CONFIG.string(), property_tree);
    } catch (const boost::property_tree::ini_parser_error& e) {
        console::out::err("[config] error while parsing file: " + std::string{e.what()});
        return false;
    }
    return true;
}

bool Config::tryToLoadValuesFromTree() {
    try {
        loadValuesFromTree();
    } catch (const boost::property_tree::ptree_error& e) {
        console::out::err("[config] error while getting values from tree: " + std::string{e.what()});
        return false;
    }
    return true;
}

void Config::loadValuesFromTree() {
    values.server_address = getValueFromTree<std::string>("server.address");
    values.server_port = getValueFromTree<uint16_t>("server.port");

    values.client_connect_at_launch = getValueFromTree<bool>("client.connect_at_launch");

    values.shell_verbose = getValueFromTree<bool>("shell.verbose");
    values.shell_print_addr_prefix = getValueFromTree<bool>("shell.print_addr_prefix");
    values.shell_language = getValueFromTree<std::string>("shell.language");

    values.cmd_download_output = getValueFromTree<std::string>("commands.download_output");
    values.cmd_download_buffer_size = getValueFromTree<uint16_t>("commands.download_buffer_size");
    values.cmd_upload_buffer_size = getValueFromTree<uint16_t>("commands.upload_buffer_size");
}

template<typename T> T Config::getValueFromTree(const std::string& key) {
    if constexpr (std::is_same_v<T, std::string>) {
        const std::string raw_val{property_tree.get<std::string>(key)};
        EnvVarStrResolver resolver{};
        resolver.setInput(raw_val);
        resolver.resolve();
        return resolver.getOutput();
    } else {
        return property_tree.get<T>(key);
    }
}

bool Config::updateFile() const {
    try {
        boost::property_tree::write_ini(dirs::D_CONFIG.string(), property_tree);
    } catch (const boost::property_tree::ini_parser_error& e) {
        console::out::err("[config] error while updating file: " + std::string{e.what()});
        return false;
    }
    return true;
}

const ConfigValues& Config::getValues() const {
    return values;
}

ConfigValues& Config::getModifiableValues() {
    return values;
}

std::string Config::getValue(const std::string& key) const {
    auto opt_val{property_tree.get_optional<std::string>(key)};
    return (opt_val && opt_val.get() != "") ? opt_val.get() : common::UNDEFINED_TEXT;
}

std::string Config::getResolvedValue(const std::string& key) const {
    auto opt_val{property_tree.get_optional<std::string>(key)};
    if (!opt_val || opt_val.get() == "") {
        return common::UNDEFINED_TEXT;
    }
    EnvVarStrResolver resolver{};
    resolver.setInput(opt_val.get());
    resolver.resolve();
    return resolver.getOutput();
}

bool Config::setValue(const std::string& key, const std::string& value) {
    property_tree.put(key, value);
    return tryToLoadValuesFromTree();
}

const boost::property_tree::ptree& Config::getPropertyTree() const {
    return property_tree;
}
