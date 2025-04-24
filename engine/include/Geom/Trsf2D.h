#pragma once
#include <iostream>

namespace geom {

struct Vector2D;

class Trsf2D
{
public:
    Trsf2D() = default;

    void setRotation (float rad);
    void setTranslation (Vector2D trsl);
    void addTranslation (Vector2D trsl);
    void addTranslation (float e, float f)
    {
        m_e += e;
        m_f += f;
    }
    inline void addTranslationX (float x) { m_e += x; }
    inline void addTranslationY (float y) { m_f += y; }

    Vector2D getTranslationPart() const;

    Vector2D operator* (const Vector2D& rhs) const;

    // Add matrices multiplication for accumulating rotation

private:
    float m_a { 1 }, m_b { 0 }, m_c { 0 }, m_d { 1 }, m_e { 0 }, m_f { 0 };
};

} // namespace geom
