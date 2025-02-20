#pragma once

namespace geom {
class Vector2D;
}
namespace input {
enum class Device;
}

namespace input {

geom::Vector2D getAxisVec();
Device         getActiveDevice();

} // namespace input
