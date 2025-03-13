#pragma once
#include <Geom/Point2D.h>

namespace physics {

class Collider
{
public:
    Collider() = delete;
    Collider (geom::Point2D min, geom::Point2D max):
        m_min (min),
        m_max (max)
    {}

    Collider (const Collider& other):
        m_min (other.m_min),
        m_max (other.m_max)
    {}

    Collider (geom::Point2D min, float height, float width):
        m_min (min),
        m_max (min.X() + width, min.Y() + height)
    {}

    [[nodiscard]] inline float Width() const { return m_max.X() - m_min.X(); }
    [[nodiscard]] inline float Height() const { return m_max.Y() - m_min.Y(); }

    [[nodiscard]] inline const geom::Point2D& Min() const { return m_min; }
    [[nodiscard]] inline const geom::Point2D& Max() const { return m_max; }

    [[nodiscard]] inline geom::Point2D RightCorner() const { return geom::Point2D (m_max.X(), m_min.Y()); }
    [[nodiscard]] inline geom::Point2D LeftCorner() const { return geom::Point2D (m_min.X(), m_max.Y()); }

    [[nodiscard]] inline Collider Translated (const geom::Vector2D& trsl) const
    {
        return Collider (m_min.Translated (trsl), m_max.Translated (trsl));
    }

    [[nodiscard]] inline bool IsOutside (const geom::Point2D& pnt)
    {
        return (pnt.X() > m_max.X() || pnt.X() < m_min.X()) && (pnt.Y() > m_max.Y() || pnt.Y() < m_min.Y());
    }
    [[nodiscard]] inline bool IsInside (const geom::Point2D& pnt)
    {
        return (pnt.X() < m_max.X() && pnt.X() > m_min.X()) && (pnt.Y() < m_max.Y() && pnt.Y() > m_min.Y());
    }

    [[nodiscard]] bool collides (const Collider& other) const;

private:
    geom::Point2D m_min;
    geom::Point2D m_max;
};

} // namespace physics
