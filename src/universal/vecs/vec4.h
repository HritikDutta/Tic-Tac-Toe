#pragma once

#include "../basic_types.h"

struct Vec4
{
    union
    {
        f32 data[4];

        struct
        {
            union { f32 x, r, s; };
            union { f32 y, g, t; };
            union { f32 z, b, u; };
            union { f32 w, a, v; };
        };
    };

    f32 SqrLength() const;
    f32 Length() const;

    Vec4 Unit() const;
    Vec4& Normalize();

    Vec4 operator+(const Vec4& right) const;
    Vec4 operator-(const Vec4& right) const;

    Vec4 operator*(f32 scalar) const;
    Vec4 operator/(f32 scalar) const;

    Vec4& operator+=(const Vec4& right);
    Vec4& operator-=(const Vec4& right);

    Vec4& operator*=(f32 scalar);
    Vec4& operator/=(f32 scalar);

    bool operator==(const Vec4& right) const;
    bool operator!=(const Vec4& right) const;
};

f32 Dot(const Vec4& v1, const Vec4& v2);

f32 SqrDistance(const Vec4& v1, const Vec4& v2);
f32 Distance(const Vec4& v1, const Vec4& v2);

Vec4 Lerp(const Vec4& a, const Vec4& b, f32 t);