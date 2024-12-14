#include <Physics/Trsf2D.h>

#include <Physics/Vector2D.h>

namespace phs {

void Trsf2D::AddTranslation (Vector2D trsl)
{
    m_e += trsl.X();
    m_f += trsl.Y();
}

Vector2D Trsf2D::GetTranslationPart() const
{
    return Vector2D (m_e, m_f);
}

Vector2D Trsf2D::operator* (const Vector2D& rhs) const
{
    float x = rhs.X() * m_a + rhs.Y() * m_c + m_e;
    float y = rhs.X() * m_b + rhs.Y() * m_d + m_f;
    return Vector2D (x, y);
}

void Trsf2D::Log (std::string prefix)
{
    if (prefix != "") {
        std::cout << prefix << " t";
    } else {
        std::cout << "T";
    }
    std::cout << "rsf matrix = ( \n"
              << m_a << " " << m_c << " " << m_e << '\n'
              << m_b << " " << m_d << " " << m_f << '\n'
              << "0 0 1\n)" << std::endl;
}

}