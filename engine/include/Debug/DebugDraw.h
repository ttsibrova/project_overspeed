#pragma once
#include <Physics/Point2D.h>
#include <Physics/Vector2D.h>
#include <raylib/raylib.h>

namespace phs {
class Collider;
}
namespace Debug
{
void Draw (const phs::Collider& collider, const Color& color = BLUE);
void Draw (const phs::Point2D& pnt1, const phs::Point2D& pnt2, const Color& color = RED);
void Draw (const phs::Point2D& pos, const phs::Vector2D& vec, const Color& color = GREEN);
void Draw (const phs::Point2D& pnt, const Color& color = RED);
}

