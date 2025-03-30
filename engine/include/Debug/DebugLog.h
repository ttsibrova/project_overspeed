#pragma once
#include <raylib.h>
#include <format>
#include <utility>
#include <string>

namespace physics {
class Collider;
}
namespace geom {
class Point2D;
class Vector2D;
} // namespace geom
namespace physics::movement {
struct UpdateState;
}

namespace Debug {

enum Designation : uint8_t
{
    Collision,
};

#if _DEBUG

void Log (const std::pair<geom::Point2D, geom::Point2D>& line, Designation des);
void Log (const physics::Collider& collider, const Color& color = BLUE);
void Log (const physics::movement::UpdateState& pus, const geom::Point2D& playerPos);
void Log (const geom::Point2D& pnt, Designation des);

void Log (const std::string& message);

#else

// Release version of functions
template <typename... Types>
inline void Log (Types...)
{}
#endif

} // namespace Debug
