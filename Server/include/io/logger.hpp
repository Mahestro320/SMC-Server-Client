#pragma once

#include <filesystem>
#include <fstream>

class Logger final {
    static constexpr const char* LOG_EXT{"log"};
    std::filesystem::path log_path{};
    std::ofstream* file{};

    void setLogPath();

    ~Logger();
    Logger() = default;

  public:
    static Logger& getInstance();

    void init();
    std::ofstream& getStream();
};
