#include <Debug/DebugDraw.h>

#include <Physics/Collider.h>

#include <cmath>
#include <format>


namespace Debug
{
#pragma region HELPERS
void DrawTriangle (const phs::Point2D& pos, float height, float rotation, const Color& color)
{
    DrawPoly (pos, 3, height * 2 / 3, rotation, color);
}


#pragma endregion HELPERS


void Draw (const Collider& collider, const Color& color)
{
    Vector2 min = collider.Min();
    Vector2 max = collider.Max();
    Vector2 downLeft {min.x, max.y};
    Vector2 topRight {max.x, min.y};

    DrawLineV (min, topRight, color);
    DrawLineV (topRight, max, color);
    DrawLineV (max, downLeft, color);
    DrawLineV (downLeft, min, color);
}

void Draw (const phs::Point2D& pnt1, const phs::Point2D& pnt2, const Color& color)
{
    DrawLineV (pnt1, pnt2, color);
}

void Draw (const phs::Point2D& pos, const phs::Vector2D& vec, const Color& color)
{
    if (vec.SquareMagnitude() < 1.e-6) {
        return;
    }
    auto oX = phs::RightVector();
    auto dir = vec.Normalized();
    float angle = std::acos (dir.Dot (oX)) * 180 / PI;

    auto endPnt = pos.Translated (dir * 40);
    DrawCircleV (pos, 4.f, color);
    DrawLineEx (pos, endPnt, 2.f, color);
    DrawTriangle (endPnt, 8.f, angle, color);

    auto vQ = phs::GetVectorQudrant (dir);
    std::string vectorText = std::format ("({:.2f}, {:.2f})", vec.X(), vec.Y());
    switch (vQ)
    {
    case phs::Quadrant::I: case phs::Quadrant::II:
    case phs::Quadrant::Y_ALIGNED:
        DrawText (vectorText.c_str(), static_cast <int> (endPnt.X()) + 8, static_cast <int> (endPnt.Y()) + 20, 15, color);
        break;
    case phs::Quadrant::III: case phs::Quadrant::IV:
    case phs::Quadrant::X_ALIGNED: case phs::Quadrant::X_OPPOSITE:
    case phs::Quadrant::Y_OPPOSITE:
    {
        DrawText (vectorText.c_str(), static_cast <int> (endPnt.X()) + 8, static_cast <int> (endPnt.Y()) - 20, 15, color);
    }
        break;

    }
}

void Draw (const phs::Point2D& pnt, const Color& color)
{
    DrawCircleV (pnt, 1.f, color);
}

}
