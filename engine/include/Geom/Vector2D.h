#pragma once
#include <Geom/Precision.h>
#include <cassert>
#include <iostream>
#include <raylib/raylib.h>

namespace geom {

enum class Axis : uint8_t
{
    X,
    Y
};

/* Quadrants
            |
     III    |    IV
            |
    ------------------> X
            |
      II    |    I
            |
            V
             Y
*/
enum class Quadrant : uint8_t
{
    I,
    II,
    III,
    IV,
    X_ALIGNED,
    Y_ALIGNED,
    X_OPPOSITE,
    Y_OPPOSITE
};

class Trsf2D;

class Vector2D
{
public:
    Vector2D():
        m_XY { 0.f, 0.f }
    {}

    Vector2D (float x, float y):
        m_XY { x, y }
    {}

    inline float  X() const { return m_XY.x; }
    inline float& X() { return m_XY.x; }
    inline float  Y() const { return m_XY.y; }
    inline float& Y() { return m_XY.y; }

public:
    inline Vector2D operator+ (const Vector2D& rhs) const { return Vector2D (X() + rhs.X(), Y() + rhs.Y()); }

    inline void operator+= (const Vector2D& rhs)
    {
        m_XY.x += rhs.X();
        m_XY.y += rhs.Y();
    }

    inline Vector2D operator- (const Vector2D& rhs) const { return Vector2D (X() - rhs.X(), Y() - rhs.Y()); }

    inline Vector2D operator-= (const Vector2D& rhs)
    {
        m_XY.x -= rhs.X();
        m_XY.y -= rhs.Y();
    }

    inline Vector2D operator* (const float scalar) const { return Vector2D (X() * scalar, Y() * scalar); }

    [[nodiscard]] inline float Dot (const Vector2D& other) const { return X() * other.X() + Y() * other.Y(); }

    [[nodiscard]] inline float SquareMagnitude() const { return X() * X() + Y() * Y(); }

    [[nodiscard]] inline float Magnitude() const { return std::sqrt (SquareMagnitude()); }

    inline void Normalize()
    {
        m_XY.x = m_XY.x / Magnitude();
        m_XY.y = m_XY.y / Magnitude();
    }
    [[nodiscard]] inline Vector2D Normalized() const { return Vector2D (m_XY.x / Magnitude(), m_XY.y / Magnitude()); }

    inline void                   Translate (const Vector2D& trsl) { *(this) += trsl; }
    [[nodiscard]] inline Vector2D Translated (const Vector2D& trsl) const { return *(this) + trsl; }

    void                   Transform (const Trsf2D& trsf);
    [[nodiscard]] Vector2D Transformed (const Trsf2D& trsf) const;

    inline void Flip()
    {
        m_XY.x = -m_XY.x;
        m_XY.y = -m_XY.y;
    }

    [[nodiscard]] inline Vector2D Fipped() const { return Vector2D (-m_XY.x, -m_XY.y); }

    inline void FlipX() { m_XY.x = -m_XY.x; }

    inline void FlipY() { m_XY.y = -m_XY.y; }

    operator Vector2() const { return m_XY; }

private:
    Vector2 m_XY;
};

inline Vector2D getDownVector()
{
    return Vector2D (0.f, 1.f);
}
inline Vector2D getUpVector()
{
    return Vector2D (0.f, -1.f);
}
inline Vector2D getRightVector()
{
    return Vector2D (1.f, 0.f);
}

inline Vector2D getLeftVector()
{
    return Vector2D (-1.f, 0.f);
}

inline Vector2D stripByAxis (const Vector2D& vec, Axis axis)
{
    Vector2D res;
    switch (axis) {
    case Axis::X:
        res.X() = vec.X();
        break;
    case Axis::Y:
        res.Y() = vec.Y();
        break;
    default:
        break;
    }
    return res;
}

inline bool isOpposite (const Vector2D& v1, const Vector2D& v2, float precision = geom::precision::float_tol)
{
    if (v1.SquareMagnitude() < precision || v2.SquareMagnitude() < precision) {
        return false;
    }
    if (std::abs (v1.Y()) < precision && std::abs (v2.Y()) < precision) {
        return (v1.X() / v2.X()) < 0.f;
    }
    else if (std::abs (v1.X()) < precision && std::abs (v2.X()) < precision) {
        return (v1.Y() / v2.Y()) < 0.f;
    }

    if (std::abs (v1.Y()) < precision || std::abs (v2.Y()) < precision || std::abs (v1.X()) < precision ||
        std::abs (v2.X()) < precision) {
        return false;
    }

    float kx = v1.X() / v2.X();
    float ky = v1.Y() / v2.Y();

    return std::abs (kx - ky) < precision && kx < 0.f;
}

inline Quadrant getVectorQudrant (const Vector2D& vec)
{
    if (std::abs (vec.Y()) < precision::float_tol) {
        if (vec.X() > 0.f) {
            return Quadrant::X_ALIGNED;
        }
        else {
            return Quadrant::X_OPPOSITE;
        }
    }
    if (std::abs (vec.X()) < precision::float_tol) {
        if (vec.Y() > 0.f) {
            return Quadrant::Y_ALIGNED;
        }
        else {
            return Quadrant::Y_OPPOSITE;
        }
    }

    if (vec.X() > 0 && vec.Y() > 0) {
        return Quadrant::I;
    }
    if (vec.X() < 0 && vec.Y() > 0) {
        return Quadrant::II;
    }
    if (vec.X() < 0 && vec.Y() < 0) {
        return Quadrant::III;
    }
    if (vec.X() > 0 && vec.Y() < 0) {
        return Quadrant::IV;
    }
    std::unreachable();
}

inline Quadrant getInversedQuadrant (Quadrant quadrant)
{
    switch (quadrant) {
    case Quadrant::I:
        return Quadrant::III;
    case Quadrant::II:
        return Quadrant::IV;
    case Quadrant::III:
        return Quadrant::I;
    case Quadrant::IV:
        return Quadrant::II;
    case Quadrant::X_ALIGNED:
        return Quadrant::X_OPPOSITE;
    case Quadrant::Y_ALIGNED:
        return Quadrant::Y_OPPOSITE;
    case Quadrant::X_OPPOSITE:
        return Quadrant::X_ALIGNED;
    case Quadrant::Y_OPPOSITE:
        return Quadrant::Y_ALIGNED;
    }
    std::unreachable();
}

inline float computeAngleBetweenVectors (Vector2D v1, Vector2D v2) // in degree
{
    assert (v1.SquareMagnitude() > precision::quarter_pixel);
    assert (v2.SquareMagnitude() > precision::quarter_pixel);
    v1.Normalize();
    v2.Normalize();
    float dot   = v1.Dot (v2);
    float angle = std::acos (dot) * 180 / PI;
    if (v1.X() * v2.Y() - v1.Y() * v2.X() < 0) { // Z-component of cross product
        angle = -angle;
    }
    return angle;
}

} // namespace geom

template <>
struct std::formatter<geom::Vector2D>
{
    constexpr auto parse (std::format_parse_context& ctx) { return ctx.begin(); }

    auto format (const geom::Vector2D& v, std::format_context& ctx) const
    {
        return std::format_to (ctx.out(), "<{}, {}>", v.X(), v.Y());
    }
};
