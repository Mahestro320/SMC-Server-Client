#pragma once

#include <cstdint>
#include <limits>
#include <string>

#include "Shared/pds.hpp"
#include "shell/system/cursor.hpp"
#include "shell/util/textual_loading_bar.hpp"
#include "util/vector2.hpp"

class IOLoadInfoPrinter final {
  private:
    using bar_value_t = pds::network_buffer_count_t;

    static inline constexpr pds::timestamp_t REFRESH_TIME{1000};
    static inline constexpr uint16_t LOADING_BAR_WIDTH{30};
    static inline constexpr char INFO_SEPARATOR[]{" | "};

    Vector2<shell::cursor::range_t> begin_pos{};

    size_t last_info_str_size{};
    pds::timestamp_t last_time{};
    pds::timestamp_t now{};
    pds::network_buffer_count_t curr_buffer_count{};
    pds::network_buffer_count_t last_buffer_count{};
    pds::network_buffer_count_t total_buffer_count{};
    pds::network_buffer_t buffer_size{};
    float time_left{std::numeric_limits<float>::quiet_NaN()};
    float transmition_speed{std::numeric_limits<float>::quiet_NaN()};

    TextualLoadingBar<bar_value_t> loading_bar{LOADING_BAR_WIDTH};
    std::string main_infos{};
    std::string verbose_infos{};
    bool is_verbose{};
    bool is_first_iteration{true};

    void initPos();
    void initIsVerbose();
    void initLoadingBar();

    void updateValues();
    void updateTimeLeft(pds::network_buffer_count_t buffers_sent);
    void updateTransmitionSpeed(pds::network_buffer_count_t buffers_sent);
    void updateText();

    void printText();

    void clearInfoStr() const;
    void clearLoadingBar() const;

  public:
    IOLoadInfoPrinter() = default;

    void init();
    void update();
    void clear();

    void setCurrentBufferCount(pds::network_buffer_count_t count);
    void setTotalBufferCount(pds::network_buffer_count_t count);
    void setBufferSize(pds::network_buffer_t size);
};
