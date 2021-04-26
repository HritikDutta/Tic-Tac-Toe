// @Read https://lxjk.github.io/2016/10/29/A-Different-Way-to-Understand-Quaternion-and-Rotation.html

#pragma once

#include "../basic_types.h"
#include "../vecs/vec3.h"
#include "../vecs/vec4.h"

struct Mat4
{
    union
    {
        Vec4 cols[4];
        f32  data[16];
    };

    static Mat4 Identity();
    static Mat4 Diagonal(f32 d1, f32 d2, f32 d3, f32 d4);
    static Mat4 Diagonal(f32 diag);

    // Affine Transformations
    static Mat4 Translation(const Vec3& ds);
    static Mat4 Rotation(Vec3 axis, f32 angle);
    static Mat4 Scaling(const Vec3& scale);

    // Camera Transformations
    static Mat4 Perspective(f32 fov, f32 aspectRatio, f32 near, f32 far);
    static Mat4 Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);

    static Mat4 LookAt(Vec3 eye, Vec3 center, Vec3 up);

    Mat4& Translate(const Vec3& ds);
    Mat4& Rotate(Vec3 axis, f32 angle);
    Mat4& Scale(const Vec3& scale);

    Mat4 operator+(const Mat4& right) const;
    Mat4 operator-(const Mat4& right) const;
    Mat4 operator*(const Mat4& right) const;

    Vec3 operator*(const Vec3& vec) const;
    Vec4 operator*(const Vec4& vec) const;

    Mat4 operator*(f32 scalar) const;

    Mat4& operator+=(const Mat4& right);
    Mat4& operator-=(const Mat4& right);
    Mat4& operator*=(const Mat4& right);    // A *= B == B * A; This way makes more sense to me

    Mat4& operator*=(f32 scalar);
};