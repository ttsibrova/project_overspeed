#include <Physics/Vector2D.h>

#include <Physics/Trsf2D.h>


namespace phs {

Vector2D phs::Vector2D::Transformed (const Trsf2D& trsf) const
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
