#pragma once
#include <raylib.h>
#include <format>
#include <utility>
#include <string>

namespace physics {
class Collider;
}
namespace geom {
struct LineSegment;
struct Point2D;
struct Vector2D;
} // namespace geom
namespace physics::movement {
struct UpdateState;
}

namespace debug {

enum Designation : uint8_t
{
    Collision,
};

#if _DEBUG

void log (const geom::LineSegment& segment, Designation des);
void log (const physics::Collider& collider, const Color& color = BLUE);
void log (const physics::movement::UpdateState& pus, const geom::Point2D& playerPos);
void log (const geom::Point2D& pnt, Designation des);
void log (const std::string& message);

template <typename... Ts>
void log (std::format_string<Ts...>&& fstr, Ts&&... args)
{
    log(std::format(fstr, std::forward<Ts>(args)...));
}


#else

// Release version of functions
template <typename... Types>
inline void log (Types...)
{}
#endif

} // namespace debug
