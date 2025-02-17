#pragma once
#include <raylib/raylib.h>
#include <utility>

namespace phs
{
class Collider;
class Point2D;
class Vector2D;
}
struct PhysicsUpdateState;


namespace Debug
{

enum Designation : uint8_t
{
    Collision,
};

#if _DEBUG

void Log (const std::pair <phs::Point2D, phs::Point2D>& line, Designation des);
void Log (const phs::Collider& collider, const Color& color = BLUE);
void Log (const PhysicsUpdateState& pus, const phs::Point2D& playerPos);
void Log (const phs::Point2D& pnt, Designation des);

#elif 

// Release version of functions
template <typename ... Types>
inline void Log (Types ...) {}
#endif


}
