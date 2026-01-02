#pragma once

#include <cstdint>

#include "util/vector2.hpp"

namespace shell::cursor {

using range_t = int16_t;

extern Vector2<range_t> getPosition();
extern void move(const Vector2<range_t>& pos);

extern void moveUp(range_t count = 1);
extern void moveDown(range_t count = 1);
extern void _return();

extern void setVisible(bool visible);

} // namespace shell::cursor
