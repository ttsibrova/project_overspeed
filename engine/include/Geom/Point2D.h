#pragma once
#include <Geom/Vector2D.h>

#include <raylib/raylib.h>

#include <format>

namespace geom {

class Point2D
{
public:
    Point2D():
        m_x (0),
        m_y (0)
    {}

    Point2D (float x, float y):
        m_x (x),
        m_y (y)
    {}

    inline float  X() const { return m_x; }
    inline float& X() { return m_x; }
    inline float  Y() const { return m_y; }
    inline float& Y() { return m_y; }

    inline void Translate (const geom::Vector2D& trsl)
    {
        m_x += trsl.X();
        m_y += trsl.Y();
    }

    [[nodiscard]] inline Point2D Translated (const geom::Vector2D& trsl) const
    {
        return Point2D (m_x + trsl.X(), m_y + trsl.Y());
    }

    bool operator== (const Point2D& other) const
    {
        return std::abs (m_x - other.m_x) < 1.e-7f && std::abs (m_y - other.m_y) < 1.e-7f;
    }

    operator Vector2() const { return { m_x, m_y }; }

private:
    float m_x;
    float m_y;
};

} // namespace geom

template <>
struct std::formatter<geom::Point2D>
{
    constexpr auto parse (std::format_parse_context& ctx) { return ctx.begin(); }

    auto format (const geom::Point2D& p, std::format_context& ctx) const
    {
        return std::format_to (ctx.out(), "({:.2f}, {:.2f})\n", p.X(), p.Y());
    }
};

