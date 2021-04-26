#include "vec2.h"

#include <cmath>
#include "../basic_types.h"
#include "../math.h"

inline f32 Vec2::SqrLength() const
{
    return x * x + y * y;
}

inline f32 Vec2::Length() const
{
    return sqrtf(x * x + y * y);
}

inline Vec2 Vec2::Unit() const
{
    return *this / Length();
}

inline Vec2& Vec2::Normalize()
{
    *this /= Length();
    return *this;
}

Vec2 Vec2::operator+(const Vec2& right) const
{
    return Vec2 { x + right.x, y + right.y };
}

Vec2 Vec2::operator-(const Vec2& right) const
{
    return Vec2 { x - right.x, y - right.y };
}

Vec2 Vec2::operator*(const Vec2& right) const
{
    return Vec2 { x * right.x, y * right.y };
}

Vec2 Vec2::operator/(const Vec2& right) const
{
    return Vec2 { x / right.x, y / right.y };
}

Vec2 Vec2::operator*(f32 scalar) const
{
    return Vec2 { x * scalar, y * scalar };
}

Vec2 Vec2::operator/(f32 scalar) const
{
    return Vec2 { x / scalar, y / scalar };
}

Vec2& Vec2::operator+=(const Vec2& right)
{
    x += right.x;
    y += right.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& right)
{
    x -= right.x;
    y -= right.y;
    return *this;
}

Vec2& Vec2::operator*=(f32 scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vec2& Vec2::operator/=(f32 scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

inline bool Vec2::operator==(const Vec2& right) const
{
    return x == right.x && y == right.y;
}

inline bool Vec2::operator!=(const Vec2& right) const
{
    return x != right.x || y != right.y;
}

inline f32 Dot(const Vec2& v1, const Vec2& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

inline f32 SqrDistance(const Vec2& v1, const Vec2& v2)
{
    return (v1 - v2).SqrLength();
}

inline f32 Distance(const Vec2& v1, const Vec2& v2)
{
    return (v1 - v2).Length();
}

Vec2 Lerp(const Vec2& a, const Vec2& b, f32 t)
{
    t = Clamp(t, 0.0f, 1.0f);
    f32 _1_t = 1.0f - t;
 
    Vec2 res;

    res.x = t * a.x + _1_t * b.x;
    res.y = t * a.y + _1_t * b.y;

    return res;
}