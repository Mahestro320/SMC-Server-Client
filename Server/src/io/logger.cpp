#include "io/logger.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <termcolor.hpp>

#include "io/directories.hpp"
#include "util/time.hpp"

namespace fs = std::filesystem;

Logger& Logger::getInstance() {
    static Logger logger;
    return logger;
}

Logger::~Logger() {
    if (file) {
        file->close();
        delete file;
    }
}

void Logger::init() {
    if (!fs::exists(dirs::logs)) {
        fs::create_directories(dirs::logs);
    }
    setLogPath();
    file = new std::ofstream{log_path, std::ios::app};
    if (!*file) {
        std::cerr << termcolor::red << "[logger] error: can't open log file" << log_path << termcolor::reset
                  << std::endl;
    }
}

void Logger::setLogPath() {
    log_path = dirs::logs / (std::to_string(util::time::getMillis()) + "." + std::string{LOG_EXT});
}

std::ofstream& Logger::getStream() {
    return *file;
}
