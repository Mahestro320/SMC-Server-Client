#include "shell/commands/io_load/info_printer.hpp"

#include <cmath>
#include <cstdint>
#include <limits>
#include <string>

#include "Shared/io/console.hpp"
#include "Shared/pds.hpp"
#include "Shared/util/time.hpp"
#include "shell/config.hpp"
#include "shell/system/cursor.hpp"
#include "util/byte.hpp"
#include "util/string.hpp"

using cur_range_t = shell::cursor::range_t;
using namespace util::string;

void IOLoadInfoPrinter::init() {
    initPos();
    initIsVerbose();
    initLoadingBar();
}

void IOLoadInfoPrinter::initPos() {
    console::out::inf("\n");
    begin_pos = shell::cursor::getPosition();
    begin_pos.y -= 2;
}

void IOLoadInfoPrinter::initIsVerbose() {
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    is_verbose = config_values.shell_verbose;
}

void IOLoadInfoPrinter::initLoadingBar() {
    loading_bar.setMaximum(static_cast<bar_value_t>(total_buffer_count));
}

void IOLoadInfoPrinter::update() {
    if (is_first_iteration) {
        is_first_iteration = false;
        updateText();
        printText();
        last_time = util::time::getMillis();
        return;
    }
    now = util::time::getMillis();
    if (now - last_time < REFRESH_TIME) {
        return;
    }
    updateValues();
    updateText();
    printText();
}

void IOLoadInfoPrinter::clear() {
    shell::cursor::move(begin_pos);
    clearInfoStr();
    console::out::inf();
    clearLoadingBar();
    shell::cursor::move(begin_pos);
}

void IOLoadInfoPrinter::updateValues() {
    const pds::network_buffer_count_t buffers_sent{curr_buffer_count - last_buffer_count};
    updateTimeLeft(buffers_sent);
    updateTransmitionSpeed(buffers_sent);
    loading_bar.setValue(static_cast<bar_value_t>(curr_buffer_count));
    last_time = now;
    last_buffer_count = curr_buffer_count;
}

void IOLoadInfoPrinter::updateTimeLeft(pds::network_buffer_count_t buffers_sent) {
    const pds::network_buffer_count_t buffers_left{total_buffer_count - curr_buffer_count};
    const pds::timestamp_t time_elapsed{now - last_time};

    if (buffers_sent == 0) {
        this->time_left = std::numeric_limits<float>::infinity();
    }

    const float time_to_send_one_buffer{static_cast<float>(time_elapsed) / buffers_sent};
    const float time_left{time_to_send_one_buffer * buffers_left};

    this->time_left = time_left / 1000.f;
}

void IOLoadInfoPrinter::updateTransmitionSpeed(pds::network_buffer_count_t buffers_sent) {
    transmition_speed = static_cast<float>(buffers_sent * buffer_size);
}

void IOLoadInfoPrinter::updateText() {
    main_infos =
        "eta: " + ((std::isnan(time_left)) ? "???" : util::string::secondsToDHMS(static_cast<uint64_t>(time_left))) +
        INFO_SEPARATOR;
    if (std::isnan(transmition_speed)) {
        main_infos += "??? Mb/s";
    } else {
        main_infos += util::byte::toAutoUnit(static_cast<uint64_t>(transmition_speed)) + "/s";
    }

    verbose_infos = "buffer " + std::to_string(curr_buffer_count) + CHAR_SLASH + std::to_string(total_buffer_count);
}

void IOLoadInfoPrinter::printText() {
    shell::cursor::move(begin_pos);
    std::string info_str{main_infos};
    if (is_verbose) {
        info_str += INFO_SEPARATOR + verbose_infos;
    }
    const size_t info_str_size{info_str.size()};
    if (last_info_str_size > info_str_size) {
        clearInfoStr();
    }
    last_info_str_size = info_str_size;
    shell::cursor::move(begin_pos);
    console::out::inf(info_str);
    console::out::inf(loading_bar.getText());
}

void IOLoadInfoPrinter::clearInfoStr() const {
    console::out::inf(makeFilledString(last_info_str_size, CHAR_SPACE), false);
}

void IOLoadInfoPrinter::clearLoadingBar() const {
    console::out::inf(makeFilledString(LOADING_BAR_WIDTH, CHAR_SPACE), false);
}

void IOLoadInfoPrinter::setCurrentBufferCount(pds::network_buffer_count_t count) {
    this->curr_buffer_count = count;
}

void IOLoadInfoPrinter::setTotalBufferCount(pds::network_buffer_count_t count) {
    this->total_buffer_count = count;
}

void IOLoadInfoPrinter::setBufferSize(pds::network_buffer_t size) {
    this->buffer_size = size;
}
