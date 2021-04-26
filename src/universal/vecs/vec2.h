#pragma once

#include "../basic_types.h"

struct Vec2
{
    f32 x, y;

    f32 SqrLength() const;
    f32 Length() const;

    Vec2 Unit() const;
    Vec2& Normalize();

    Vec2 operator+(const Vec2& right) const;
    Vec2 operator-(const Vec2& right) const;
    Vec2 operator*(const Vec2& right) const;
    Vec2 operator/(const Vec2& right) const;

    Vec2 operator*(f32 scalar) const;
    Vec2 operator/(f32 scalar) const;

    Vec2& operator+=(const Vec2& right);
    Vec2& operator-=(const Vec2& right);

    Vec2& operator*=(f32 scalar);
    Vec2& operator/=(f32 scalar);

    bool operator==(const Vec2& right) const;
    bool operator!=(const Vec2& right) const;
};

f32 Dot(const Vec2& v1, const Vec2& v2);

f32 SqrDistance(const Vec2& v1, const Vec2& v2);
f32 Distance(const Vec2& v1, const Vec2& v2);

Vec2 Lerp(const Vec2& a, const Vec2& b, f32 t);