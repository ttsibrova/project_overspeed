#pragma once
#include <raylib/raylib.h>
#include <iostream>

namespace phs
{

enum class EAxis
{
    X,
    Y
};

class Trsf2D;

class Vector2D
{
public:

    Vector2D (float x = 0, float y = 0):
        m_XY {x, y}
    {}


    inline float  X() const { return m_XY.x; }
    inline float& X() { return m_XY.x; }
    inline float  Y() const { return m_XY.y; }
    inline float& Y() { return m_XY.y; }

public:
    inline Vector2D operator+ (const Vector2D& rhs) const {
        return Vector2D (X() + rhs.X(), Y() + rhs.Y());
    }

    inline void operator+= (const Vector2D& rhs) {
        m_XY.x += rhs.X();
        m_XY.y += rhs.Y();
    }

    inline Vector2D operator- (const Vector2D& rhs) const {
        return  Vector2D (X() - rhs.X(), Y() - rhs.Y());
    }

    inline Vector2D operator-= (const Vector2D& rhs) {
        m_XY.x -= rhs.X();
        m_XY.y -= rhs.Y();
    }

    inline Vector2D operator* (const float scalar) const {
        return Vector2D (X() * scalar, Y() * scalar);
    }

    [[nodiscard]] inline float SquareMagnitude() {
        return X() * X() + Y() * Y();
    }

    [[nodiscard]] inline float Magnitude() {
        return std::sqrt (SquareMagnitude());
    }

    inline void Normalize() {
        m_XY.x = m_XY.x / Magnitude();
        m_XY.y = m_XY.y / Magnitude();
    }

    inline void Translate (const Vector2D& trsl) {
        *(this) += trsl;
    }
    [[nodiscard]] inline Vector2D Translated (const Vector2D& trsl) const {
        return *(this) + trsl;
    }

    void Transform (const Trsf2D& trsf);
    [[nodiscard]] Vector2D Transformed (const Trsf2D& trsf) const;

    inline void Flip() {
        m_XY.x = -m_XY.x;
        m_XY.y = -m_XY.y;
    }

    inline void FlipX() {
        m_XY.x = -m_XY.x;
    }

    inline void FlipY() {
        m_XY.y = -m_XY.y;
    }

    operator Vector2() const {
        return {X(), Y()};
    }

private:
    Vector2 m_XY;
};

inline Vector2D StripByAxis (const Vector2D& vec, EAxis axis)
{
    Vector2D res;
    switch (axis)
    {
    case phs::EAxis::X:
        res.X() = vec.X();
        break;
    case phs::EAxis::Y:
        res.Y() = vec.Y();
        break;
    default:
        break;
    }
    return res;
}

inline void Log (const Vector2D& vec, std::string prefix = "") {
    std::cout << prefix << "(X Y) = (" << vec.X() << " " << vec.Y() << ")" << std::endl;
}

}

