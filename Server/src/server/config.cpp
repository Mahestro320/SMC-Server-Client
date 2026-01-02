#include "server/config.hpp"

#include <boost/property_tree/exceptions.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <string>

#include "Shared/pds.hpp"
#include "io/directories.hpp"
#include "termcolor.hpp"

Config& Config::getInstance() {
    static Config instance{};
    return instance;
}

bool Config::load() {
    std::cout << "[config] loading file\n";
    if (!loadIniFile() || !tryToLoadValuesFromTree()) {
        return false;
    }
    setDirsValues();
    return true;
}

bool Config::loadIniFile() {
    try {
        boost::property_tree::ini_parser::read_ini(dirs::D_CONFIG.string(), property_tree);
    } catch (const boost::property_tree::ini_parser_error& e) {
        std::cerr << termcolor::red << "[config] error while parsing file: " << e.what() << termcolor::reset
                  << std::endl;
        return false;
    }
    return true;
}

bool Config::tryToLoadValuesFromTree() {
    try {
        loadValuesFromTree();
    } catch (const boost::property_tree::ptree_error& e) {
        std::cerr << termcolor::red << "[config] error while getting values from tree: " << e.what() << termcolor::reset
                  << std::endl;
        return false;
    }
    return true;
}

void Config::loadValuesFromTree() {
    values.server_port = getValueFromTree<pds::connection_port_t>("server.port");

    values.dirs_storage = getValueFromTree<std::string>("dirs.storage");
    values.dirs_logs = getValueFromTree<std::string>("dirs.logs");

    values.client_min_version.setFromString(getValueFromTree<std::string>("client.min_version"));
    values.client_max_version.setFromString(getValueFromTree<std::string>("client.max_version"));

    values.time_format = getValueFromTree<std::string>("time.format");

    values.io_load_max_buffer_size = getValueFromTree<pds::network_buffer_t>("io_load.max_buffer_size");
}

void Config::setDirsValues() const {
    dirs::setLogs(values.dirs_logs);
    dirs::setStorage(values.dirs_storage);
}

const ConfigValues& Config::getValues() const {
    return values;
}
