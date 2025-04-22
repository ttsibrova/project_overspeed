#include <Geom/Trsf2D.h>

#include <Geom/Vector2D.h>

namespace geom {

void Trsf2D::setRotation (float rad)
{
    m_a = std::cos (rad);
    m_b = std::sin (rad);
    m_c = -m_b;
    m_d = m_a;
}

void Trsf2D::addTranslation (Vector2D trsl)
{
    m_e += trsl.X();
    m_f += trsl.Y();
}

void Trsf2D::setTranslation (Vector2D trsl)
{
    m_e = trsl.X();
    m_f = trsl.Y();
}

Vector2D Trsf2D::getTranslationPart() const
{
    return Vector2D (m_e, m_f);
}

Vector2D Trsf2D::operator* (const Vector2D& rhs) const
{
    float x = rhs.X() * m_a + rhs.Y() * m_c + m_e;
    float y = rhs.X() * m_b + rhs.Y() * m_d + m_f;
    return Vector2D (x, y);
}

} // namespace geom
