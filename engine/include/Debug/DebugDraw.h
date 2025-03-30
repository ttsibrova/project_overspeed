#pragma once
#include <Geom/Point2D.h>
#include <Geom/Vector2D.h>
#include <raylib.h>

namespace physics {
class Collider;
}

namespace Debug {

void draw (const physics::Collider& collider, const Color& color);
void draw (const geom::Point2D& pnt1, const geom::Point2D& pnt2, const Color& color = RED);
void draw (const geom::Point2D& pos, const geom::Vector2D& vec, const Color& color = GREEN);
void draw (const geom::Point2D& pnt, const Color& color = RED);

} // namespace Debug
