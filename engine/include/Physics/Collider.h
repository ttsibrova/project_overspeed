#pragma once
#include <Geom/Point2D.h>

namespace physics {

class Collider
{
public:
    Collider() = delete;

    Collider (geom::Point2D min, geom::Point2D max):
        m_min { std::move (min) },
        m_max { std::move (max) }
    {}

    Collider (const Collider& other):
        m_min { other.m_min },
        m_max { other.m_max }
    {}

    Collider (geom::Point2D min, float height, float width):
        m_min { min },
        m_max { .x = min.x + width, .y = min.y + height }
    {}

    [[nodiscard]] inline float getWidth() const { return m_max.x - m_min.x; }
    [[nodiscard]] inline float getHeight() const { return m_max.y - m_min.y; }

    [[nodiscard]] inline const geom::Point2D& getMin() const { return m_min; }
    [[nodiscard]] inline const geom::Point2D& getMax() const { return m_max; }

    [[nodiscard]] inline geom::Point2D getRightCorner() const { return geom::Point2D (m_max.x, m_min.y); }
    [[nodiscard]] inline geom::Point2D getLeftCorner() const { return geom::Point2D (m_min.x, m_max.y); }

    [[nodiscard]] inline Collider translated (const geom::Vector2D& trsl) const
    {
        return Collider (m_min.translated (trsl), m_max.translated (trsl));
    }

    [[nodiscard]] inline bool contains (const geom::Point2D& pnt)
    {
        return (pnt.x < m_max.x && pnt.x > m_min.x) && (pnt.y < m_max.y && pnt.y > m_min.y);
    }

    [[nodiscard]] bool collides (const Collider& other) const;

private:
    geom::Point2D m_min;
    geom::Point2D m_max;
};

} // namespace physics
