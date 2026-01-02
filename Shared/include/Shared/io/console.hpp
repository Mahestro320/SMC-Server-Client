#pragma once

#include <exception>
#include <string>

namespace console {

extern void init();

namespace in {

extern std::string getLine();

} // namespace in

namespace out {

extern void inf(const std::string& msg = "", bool line_return = true);
extern void warn(const std::string& msg = "", bool line_return = true);
extern void err(const std::string& msg = "", bool line_return = true);
extern void err(const std::exception& e, bool line_return = true);
extern void verbose(const std::string& msg = "", bool line_return = true);

} // namespace out

} // namespace console
