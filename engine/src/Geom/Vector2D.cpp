#include <Geom/Vector2D.h>

#include <Geom/Trsf2D.h>

namespace geom {

Vector2D Vector2D::transformed (const Trsf2D& trsf) const
{
    return trsf * *(this);
}

void Vector2D::transform (const Trsf2D& trsf)
{
    auto res = trsf * *(this);
    x   = res.x;
    y   = res.y;
}

} // namespace geom
