#include <Geom/Vector2D.h>

#include <Geom/Trsf2D.h>


namespace geom {

Vector2D Vector2D::Transformed (const Trsf2D& trsf) const
{
    return trsf * *(this);
}

void Vector2D::Transform (const Trsf2D& trsf)
{
    auto res = trsf * *(this);
    m_XY.x = res.X();
    m_XY.y = res.Y();
}

}
