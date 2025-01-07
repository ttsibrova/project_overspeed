#pragma once
#include <iostream>

namespace phs {

class Vector2D;

class Trsf2D
{
public:
    Trsf2D() = default;

    void SetRotation (float rad);
    void AddRotation (float a, float b, float c, float d) {
        m_a += a;
        m_b += b;
        m_c += c;
        m_d += d;
    }

    void AddTranslation (Vector2D trsl);
    void SetTranslation (Vector2D trsl);
    void AddTranslation (float e, float f) {
        m_e += e;
        m_f += f;
    }
    inline void AddTranslationX (float x) {
        m_e += x;
    }
    inline void AddTranslationY (float y) {
        m_f += y;
    }

    Vector2D GetTranslationPart() const;

    Vector2D operator* (const Vector2D& rhs) const;


private:
    float m_a{1}, m_b{0}, m_c{0}, m_d{1}, m_e{0}, m_f{0};
};

//void Log (const Trsf2D& trsf, std::string prefix = "");
}
