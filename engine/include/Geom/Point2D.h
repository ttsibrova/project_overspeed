#pragma once
#include <Geom/Vector2D.h>

#include <raylib.h>

#include <format>

namespace geom {

struct Point2D
{
    float x = 0.f;
    float y = 0.f;

    inline void translate (const geom::Vector2D& trsl)
    {
        x += trsl.x;
        y += trsl.y;
    }

    [[nodiscard]] inline Point2D translated (const geom::Vector2D& trsl) const
    {
        return Point2D { .x = x + trsl.x, .y = y + trsl.y };
    }

    bool operator== (const Point2D& other) const
    {
        return std::abs (x - other.x) < 1.e-7f && std::abs (y - other.y) < 1.e-7f;
    }

    operator Vector2() const { return { x, y }; }

};

} // namespace geom

template <>
struct std::formatter<geom::Point2D>
{
    constexpr auto parse (std::format_parse_context& ctx) { return ctx.begin(); }

    auto format (const geom::Point2D& p, std::format_context& ctx) const
    {
        return std::format_to (ctx.out(), "({:.2f}, {:.2f})\n", p.x, p.y);
    }
};

