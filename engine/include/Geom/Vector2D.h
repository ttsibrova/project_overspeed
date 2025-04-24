#pragma once
#include <Geom/Precision.h>
#include <cassert>
#include <iostream>
#include <raylib.h>

namespace geom {

enum class Axis : uint8_t
{
    x,
    y
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

struct Vector2D
{
    float x = 0.f;
    float y = 0.f;

    inline Vector2D operator+ (const Vector2D& rhs) const { return Vector2D (x + rhs.x, y + rhs.y); }

    inline void operator+= (const Vector2D& rhs)
    {
        x += rhs.x;
        y += rhs.y;
    }

    inline Vector2D operator- (const Vector2D& rhs) const { return Vector2D (x - rhs.x, y - rhs.y); }

    inline Vector2D operator-= (const Vector2D& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
    }

    inline Vector2D operator* (const float scalar) const { return Vector2D (x * scalar, y * scalar); }

    [[nodiscard]] inline float dot (const Vector2D& other) const { return x * other.x + y * other.y; }

    [[nodiscard]] inline float getSquareMagnitude() const { return x * x + y * y; }

    [[nodiscard]] inline float getMagnitude() const { return std::sqrt (getSquareMagnitude()); }

    inline void Normalize()
    {
        const float magnitude = getMagnitude();
        x = x / magnitude;
        y = y / magnitude;
    }

    [[nodiscard]] inline Vector2D normalized() const
    {
        const float magnitude = getMagnitude();
        return Vector2D (x / magnitude, y / magnitude);
    }

    inline void                   translate (const Vector2D& trsl) { *(this) += trsl; }
    [[nodiscard]] inline Vector2D translated (const Vector2D& trsl) const { return *(this) + trsl; }

    void                   transform (const Trsf2D& trsf);
    [[nodiscard]] Vector2D transformed (const Trsf2D& trsf) const;

    inline void flip()
    {
        x = -x;
        y = -y;
    }

    [[nodiscard]] inline Vector2D fipped() const { return Vector2D { .x = -x, .y = -y }; }

    inline void flipX() { x = -x; }
    inline void flipY() { y = -y; }

    operator Vector2() const { return Vector2 { .x = x, .y = y }; }
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
    Vector2D res {};
    switch (axis) {
    case Axis::x:
        res.x = vec.x;
        break;
    case Axis::y:
        res.y = vec.y;
        break;
    default:
        break;
    }
    return res;
}

inline bool isOpposite (const Vector2D& v1, const Vector2D& v2, float precision = geom::precision::floatTol)
{
    if (v1.getSquareMagnitude() < precision || v2.getSquareMagnitude() < precision) {
        return false;
    }
    if (std::abs (v1.y) < precision && std::abs (v2.y) < precision) {
        return (v1.x / v2.x) < 0.f;
    }
    else if (std::abs (v1.x) < precision && std::abs (v2.x) < precision) {
        return (v1.y / v2.y) < 0.f;
    }

    if (std::abs (v1.y) < precision || std::abs (v2.y) < precision || std::abs (v1.x) < precision ||
        std::abs (v2.x) < precision) {
        return false;
    }

    const float kx = v1.x / v2.x;
    const float ky = v1.y / v2.y;

    return std::abs (kx - ky) < precision && kx < 0.f;
}

inline Quadrant getVectorQudrant (const Vector2D& vec)
{
    if (std::abs (vec.y) < precision::floatTol) {
        if (vec.x > 0.f) {
            return Quadrant::X_ALIGNED;
        }
        else {
            return Quadrant::X_OPPOSITE;
        }
    }
    if (std::abs (vec.x) < precision::floatTol) {
        if (vec.y > 0.f) {
            return Quadrant::Y_ALIGNED;
        }
        else {
            return Quadrant::Y_OPPOSITE;
        }
    }

    if (vec.x > 0 && vec.y > 0) {
        return Quadrant::I;
    }
    if (vec.x < 0 && vec.y > 0) {
        return Quadrant::II;
    }
    if (vec.x < 0 && vec.y < 0) {
        return Quadrant::III;
    }
    if (vec.x > 0 && vec.y < 0) {
        return Quadrant::IV;
    }
    std::unreachable();
}

inline Quadrant getInversedQuadrant (const Quadrant quadrant)
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
    assert (v1.getSquareMagnitude() > precision::quarterPixel);
    assert (v2.getSquareMagnitude() > precision::quarterPixel);
    v1.Normalize();
    v2.Normalize();
    const float dot   = v1.dot (v2);
    float angle = std::acos (dot) * 180 / PI;
    if (v1.x * v2.y - v1.y * v2.x < 0) { // Z-component of cross product
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
        return std::format_to (ctx.out(), "<{}, {}>", v.x, v.y);
    }
};
