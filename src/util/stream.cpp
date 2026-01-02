#include "util/stream.hpp"

void util::stream::writeBufferInStream(std::ostream& stream, const std::vector<char>& buffer) {
    stream.write(buffer.data(), static_cast<std::streamsize>(buffer.size()));
}
