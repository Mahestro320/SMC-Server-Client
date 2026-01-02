#pragma once

#include <ostream>
#include <string>
#include <termcolor.hpp>
#include <unordered_map>

using TermcolorFunc = std::ostream& (*)(std::ostream&);

static const std::unordered_map<std::string, TermcolorFunc> termcolor_map{
    // Basic colors
    {"grey", termcolor::grey},
    {"red", termcolor::red},
    {"green", termcolor::green},
    {"yellow", termcolor::yellow},
    {"blue", termcolor::blue},
    {"magenta", termcolor::magenta},
    {"cyan", termcolor::cyan},
    {"white", termcolor::white},

    // Bright colors
    {"bright_grey", termcolor::bright_grey},
    {"bright_red", termcolor::bright_red},
    {"bright_green", termcolor::bright_green},
    {"bright_yellow", termcolor::bright_yellow},
    {"bright_blue", termcolor::bright_blue},
    {"bright_magenta", termcolor::bright_magenta},
    {"bright_cyan", termcolor::bright_cyan},
    {"bright_white", termcolor::bright_white},

    // Backgrounds
    {"on_grey", termcolor::on_grey},
    {"on_red", termcolor::on_red},
    {"on_green", termcolor::on_green},
    {"on_yellow", termcolor::on_yellow},
    {"on_blue", termcolor::on_blue},
    {"on_magenta", termcolor::on_magenta},
    {"on_cyan", termcolor::on_cyan},
    {"on_white", termcolor::on_white},

    // Bright backgrounds
    {"on_bright_grey", termcolor::on_bright_grey},
    {"on_bright_red", termcolor::on_bright_red},
    {"on_bright_green", termcolor::on_bright_green},
    {"on_bright_yellow", termcolor::on_bright_yellow},
    {"on_bright_blue", termcolor::on_bright_blue},
    {"on_bright_magenta", termcolor::on_bright_magenta},
    {"on_bright_cyan", termcolor::on_bright_cyan},
    {"on_bright_white", termcolor::on_bright_white},

    // Modificators
    {"bold", termcolor::bold},
    {"dark", termcolor::dark},
    {"underline", termcolor::underline},
    {"blink", termcolor::blink},
    {"reverse", termcolor::reverse},
    {"concealed", termcolor::concealed},
    {"reset", termcolor::reset},
};
