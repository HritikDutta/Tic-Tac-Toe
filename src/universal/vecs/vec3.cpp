#include "vec3.h"

#include <cmath>
#include "../basic_types.h"
#include "../math.h"

inline f32 Vec3::SqrLength() const
{
    return x * x + y * y + z * z;
}

inline f32 Vec3::Length() const
{
    return sqrtf(x * x + y * y + z * z);
}

Vec3 Vec3::Unit() const
{
    return *this / Length();
}

Vec3& Vec3::Normalize()
{
    *this /= Length();
    return *this;
}

Vec3 Vec3::operator+(const Vec3& right) const
{
    return Vec3 { x + right.x, y + right.y, z + right.z };
}

Vec3 Vec3::operator-(const Vec3& right) const
{
    return Vec3 { x - right.x, y - right.y, z - right.z };
}

Vec3 Vec3::operator*(f32 scalar) const
{
    return Vec3 { x * scalar, y * scalar, z * scalar };
}

Vec3 Vec3::operator/(f32 scalar) const
{
    return Vec3 { x / scalar, y / scalar, z / scalar };
}

Vec3& Vec3::operator+=(const Vec3& right)
{
    x += right.x;
    y += right.y;
    z += right.z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& right)
{
    x -= right.x;
    y -= right.y;
    z -= right.z;
    return *this;
}

Vec3& Vec3::operator*=(f32 scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vec3& Vec3::operator/=(f32 scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

inline bool Vec3::operator==(const Vec3& right) const
{
    return x == right.x && y == right.y && z == right.z;
}

inline bool Vec3::operator!=(const Vec3& right) const
{
    return x != right.x || y != right.y || z != right.z;
}

f32 Dot(const Vec3& v1, const Vec3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 Cross(const Vec3& left, const Vec3& right)
{
    Vec3 v;

    v.x = left.y * right.z - left.z * right.y;
    v.y = left.z * right.x - left.x * right.z;
    v.z = left.x * right.y - left.y * right.x;

    return v;
}

f32 SqrDistance(const Vec3& v1, const Vec3& v2)
{
    return (v1 - v2).SqrLength();
}

f32 Distance(const Vec3& v1, const Vec3& v2)
{
    return (v1 - v2).Length();
}

Vec3 Lerp(const Vec3& a, const Vec3& b, f32 t)
{
    t = Clamp(t, 0.0f, 1.0f);
    f32 _1_t = 1.0f - t;

    Vec3 res;

    res.x = t * a.x + _1_t * b.x;
    res.y = t * a.y + _1_t * b.y;
    res.z = t * a.z + _1_t * b.z;

    return res;
}