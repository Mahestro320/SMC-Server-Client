#pragma once

#include <ostream>
#include <vector>

namespace util::stream {

void writeBufferInStream(std::ostream& stream, const std::vector<char>& buffer);

} // namespace util::stream
