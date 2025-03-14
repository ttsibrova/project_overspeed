#include <Debug/DebugDraw.h>

#include <Physics/Collider.h>

#include <cmath>
#include <format>

namespace Debug {
#pragma region HELPERS
void DrawTriangle (const geom::Point2D& pos, float height, float rotation, const Color& color)
{
    DrawPoly (pos, 3, height * 2 / 3, rotation, color);
}

#pragma endregion HELPERS

void draw (const physics::Collider& collider, const Color& color)
{
    Vector2 min = collider.Min();
    Vector2 max = collider.Max();
    Vector2 downLeft { min.x, max.y };
    Vector2 topRight { max.x, min.y };

    DrawLineV (min, topRight, color);
    DrawLineV (topRight, max, color);
    DrawLineV (max, downLeft, color);
    DrawLineV (downLeft, min, color);
}

void draw (const geom::Point2D& pnt1, const geom::Point2D& pnt2, const Color& color)
{
    DrawLineV (pnt1, pnt2, color);
}

void draw (const geom::Point2D& pos, const geom::Vector2D& vec, const Color& color)
{
    if (vec.SquareMagnitude() < 1.e-6) {
        return;
    }
    auto  oX    = geom::getRightVector();
    auto  dir   = vec.Normalized();
    float angle = std::acos (dir.Dot (oX)) * 180 / PI;

    auto endPnt = pos.Translated (dir * 40);
    DrawCircleV (pos, 4.f, color);
    DrawLineEx (pos, endPnt, 2.f, color);
    DrawTriangle (endPnt, 8.f, angle, color);

    std::string vectorText = std::format ("({:.2f}, {:.2f})", vec.X(), vec.Y());
    switch (geom::getVectorQudrant (dir)) {
    case geom::Quadrant::I:
    case geom::Quadrant::II:
    case geom::Quadrant::Y_ALIGNED:
        DrawText (vectorText.c_str(), static_cast<int> (endPnt.X()) + 8, static_cast<int> (endPnt.Y()) + 20, 15, color);
        break;
    case geom::Quadrant::III:
    case geom::Quadrant::IV:
    case geom::Quadrant::X_ALIGNED:
    case geom::Quadrant::X_OPPOSITE:
    case geom::Quadrant::Y_OPPOSITE:
        DrawText (vectorText.c_str(), static_cast<int> (endPnt.X()) + 8, static_cast<int> (endPnt.Y()) - 20, 15, color);
        break;
    }
}

void draw (const geom::Point2D& pnt, const Color& color)
{
    DrawCircleV (pnt, 1.f, color);
}

} // namespace Debug
