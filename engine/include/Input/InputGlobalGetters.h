#pragma once
#include <cstdint>

namespace geom {
class Vector2D;
}
namespace input {
enum class Device : uint8_t;
}

namespace input {


geom::Vector2D getAxisVec();
Device         getActiveDevice();

} // namespace input
