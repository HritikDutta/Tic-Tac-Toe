#pragma once

#include "../basic_types.h"

struct Vec3
{
    union
    {
        f32 data[3];

        struct
        {
            union { f32 x, r; };
            union { f32 y, g; };
            union { f32 z, b; };
        };
    };

    f32 SqrLength() const;
    f32 Length() const;

    Vec3 Unit() const;
    Vec3& Normalize();

    Vec3 operator+(const Vec3& right) const;
    Vec3 operator-(const Vec3& right) const;
    
    Vec3 operator*(f32 scalar) const;
    Vec3 operator/(f32 scalar) const;

    Vec3& operator+=(const Vec3& right);
    Vec3& operator-=(const Vec3& right);

    Vec3& operator*=(f32 scalar);
    Vec3& operator/=(f32 scalar);

    bool operator==(const Vec3& right) const;
    bool operator!=(const Vec3& right) const;
};

f32 Dot(const Vec3& v1, const Vec3& v2);
Vec3 Cross(const Vec3& left, const Vec3& right);

f32 SqrDistance(const Vec3& v1, const Vec3& v2);
f32 Distance(const Vec3& v1, const Vec3& v2);

Vec3 Lerp(const Vec3& a, const Vec3& b, f32 t);