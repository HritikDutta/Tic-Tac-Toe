#include "mat4.h"

#include <cmath>
#include <xmmintrin.h>
#include "../basic_types.h"
#include "../vecs/vec3.h"
#include "../vecs/vec4.h"
#include "../math.h"

inline Mat4 Mat4::Diagonal(f32 d1, f32 d2, f32 d3, f32 d4)
{
    return Mat4 {
        Vec4 { d1 },
        Vec4 { 0.0f, d2 },
        Vec4 { 0.0f, 0.0f, d3 },
        Vec4 { 0.0f, 0.0f, 0.0f, d4 },
    };
}

Mat4 Mat4::Diagonal(f32 diag)
{
    return Diagonal(diag, diag, diag, diag);
}

Mat4 Mat4::Identity()
{
    return Diagonal(1.0f);
}

inline Mat4 Mat4::Translation(const Vec3& ds)
{
    return Mat4 {
        Vec4 { 1.0f },
        Vec4 { 0.0f, 1.0f },
        Vec4 { 0.0f, 0.0f, 1.0f },
        Vec4 { ds.x, ds.y, ds.z, 1.0f },
    };
}

inline Mat4 Mat4::Rotation(Vec3 axis, f32 angle)
{
    Mat4 m = Diagonal(1.0f);

    axis.Normalize();

    f32 st = sinf(angle);
    f32 ct = cosf(angle);
    f32 _1_ct = 1.0f - ct;

    m.cols[0].data[0] = (axis.x * axis.x * _1_ct) + ct;
    m.cols[0].data[1] = (axis.x * axis.y * _1_ct) + axis.z * st;
    m.cols[0].data[2] = (axis.x * axis.z * _1_ct) - axis.y * st;

    m.cols[1].data[0] = (axis.y * axis.x * _1_ct) - axis.z * st;
    m.cols[1].data[1] = (axis.y * axis.y * _1_ct) + ct;
    m.cols[1].data[2] = (axis.y * axis.z * _1_ct) + axis.x * st;

    m.cols[2].data[0] = (axis.z * axis.x * _1_ct) + axis.y * st;
    m.cols[2].data[1] = (axis.z * axis.y * _1_ct) - axis.x * st;
    m.cols[2].data[2] = (axis.z * axis.z * _1_ct) + ct;

    return m;
}

inline Mat4 Mat4::Scaling(const Vec3& scale)
{
    return Diagonal(scale.x, scale.y, scale.z, 1.0f);
}

Mat4 Mat4::Perspective(f32 fov, f32 aspectRatio, f32 near, f32 far)
{
    Mat4 m {};

    f32 cotangent = 1.0f / tanf(fov * _PI / 360.0f);

    m.cols[0].data[0] = cotangent / aspectRatio;
    m.cols[1].data[1] = cotangent;
    m.cols[2].data[2] = (near + far) / (near - far);
    m.cols[2].data[3] = -1.0f;
    m.cols[3].data[2] = (2.0f * near * far) / (near - far);

    return m;
}

Mat4 Mat4::Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far)
{
    Mat4 m {};

    m.cols[0].data[0] = 2.0f / (right - left);
    m.cols[1].data[1] = 2.0f / (top - bottom);
    m.cols[1].data[1] = 2.0f / (near - far);
    m.cols[3].data[3] = 1.0f;

    m.cols[3].data[0] = (left + right) / (left - right);
    m.cols[3].data[1] = (bottom + top) / (bottom - top);
    m.cols[3].data[2] = (far + near) / (near - far);

    return m;
}

Mat4 Mat4::LookAt(Vec3 eye, Vec3 center, Vec3 up)
{
    Mat4 m {};

    Vec3 f = (center - eye).Normalize();
    Vec3 s = Cross(f, up);
    Vec3 u = Cross(s, f);

    m.cols[0].data[0] = s.x;
    m.cols[0].data[1] = u.x;
    m.cols[0].data[2] = -f.x;

    m.cols[1].data[0] = s.y;
    m.cols[1].data[1] = u.y;
    m.cols[1].data[2] = -f.y;

    m.cols[2].data[0] = s.z;
    m.cols[2].data[1] = u.z;
    m.cols[2].data[2] = -f.z;

    m.cols[3].data[0] = Dot(s, eye) * -1.0f;
    m.cols[3].data[1] = Dot(u, eye) * -1.0f;
    m.cols[3].data[2] = Dot(f, eye);
    m.cols[3].data[3] = 1.0f;

    return m;
}

Mat4& Mat4::Translate(const Vec3& ds)
{
    *this *= Translation(ds);
    return *this;
}

Mat4& Mat4::Rotate(Vec3 axis, f32 angle)
{
    *this *= Rotation(axis, angle);
    return *this;
}

Mat4& Mat4::Scale(const Vec3& scale)
{
    *this *= Scaling(scale);
    return *this;
}

Mat4 Mat4::operator+(const Mat4& right) const
{
    Mat4 m;

    __m128 mCol, rCol;

    // Column 1
    mCol = _mm_load_ps(cols[0].data);
    rCol = _mm_load_ps(right.cols[0].data);
    _mm_store_ps(m.cols[0].data, _mm_add_ps(mCol, rCol));

    // Column 2
    mCol = _mm_load_ps(cols[1].data);
    rCol = _mm_load_ps(right.cols[1].data);
    _mm_store_ps(m.cols[1].data, _mm_add_ps(mCol, rCol));

    // Column 3
    mCol = _mm_load_ps(cols[2].data);
    rCol = _mm_load_ps(right.cols[2].data);
    _mm_store_ps(m.cols[2].data, _mm_add_ps(mCol, rCol));

    // Column 4
    mCol = _mm_load_ps(cols[3].data);
    rCol = _mm_load_ps(right.cols[3].data);
    _mm_store_ps(m.cols[3].data, _mm_add_ps(mCol, rCol));

    return m;
}

Mat4 Mat4::operator-(const Mat4& right) const
{
    Mat4 m;

    __m128 mCol, rCol;

    // Column 1
    mCol = _mm_load_ps(cols[0].data);
    rCol = _mm_load_ps(right.cols[0].data);
    _mm_store_ps(m.cols[0].data, _mm_sub_ps(mCol, rCol));

    // Column 2
    mCol = _mm_load_ps(cols[1].data);
    rCol = _mm_load_ps(right.cols[1].data);
    _mm_store_ps(m.cols[1].data, _mm_sub_ps(mCol, rCol));

    // Column 3
    mCol = _mm_load_ps(cols[2].data);
    rCol = _mm_load_ps(right.cols[2].data);
    _mm_store_ps(m.cols[2].data, _mm_sub_ps(mCol, rCol));

    // Column 4
    mCol = _mm_load_ps(cols[3].data);
    rCol = _mm_load_ps(right.cols[3].data);
    _mm_store_ps(m.cols[3].data, _mm_sub_ps(mCol, rCol));

    return m;
}

Mat4 Mat4::operator*(const Mat4& right) const
{
    Mat4 m;

    m.cols[0] = *this * right.cols[0];
    m.cols[1] = *this * right.cols[1];
    m.cols[2] = *this * right.cols[2];
    m.cols[3] = *this * right.cols[3];

    return m;
}

Vec3 Mat4::operator*(const Vec3& vec) const
{
    Vec4 v4 { vec.x, vec.y, vec.z, 1.0f };
    v4 = *this * v4;
    return Vec3 { v4.x, v4.y, v4.z };
}

Vec4 Mat4::operator*(const Vec4& vec) const
{
    Vec4 v;

    __m128 c1 = _mm_load_ps(cols[0].data);
    __m128 c2 = _mm_load_ps(cols[1].data);
    __m128 c3 = _mm_load_ps(cols[2].data);
    __m128 c4 = _mm_load_ps(cols[3].data);

    __m128 res;
    res = _mm_mul_ps(c1, _mm_set1_ps(vec.x));
    res = _mm_add_ps(res, _mm_mul_ps(c2, _mm_set1_ps(vec.y)));
    res = _mm_add_ps(res, _mm_mul_ps(c3, _mm_set1_ps(vec.z)));
    res = _mm_add_ps(res, _mm_mul_ps(c4, _mm_set1_ps(vec.w)));

    _mm_store_ps(v.data, res);
    return v;
}

Mat4 Mat4::operator*(f32 scalar) const
{
    Mat4 m;

    __m128 s = _mm_set1_ps(scalar);
    __m128 col;

    col = _mm_load_ps(cols[0].data);
    _mm_store_ps(m.cols[0].data, _mm_mul_ps(col, s));

    col = _mm_load_ps(cols[1].data);
    _mm_store_ps(m.cols[1].data, _mm_mul_ps(col, s));
    
    col = _mm_load_ps(cols[2].data);
    _mm_store_ps(m.cols[2].data, _mm_mul_ps(col, s));
    
    col = _mm_load_ps(cols[3].data);
    _mm_store_ps(m.cols[3].data, _mm_mul_ps(col, s));

    return m;
}

Mat4& Mat4::operator+=(const Mat4& right)
{
    __m128 c1, c2;

    c1 = _mm_load_ps(cols[0].data);
    c2 = _mm_load_ps(right.cols[0].data);
    _mm_store_ps(cols[0].data, _mm_add_ps(c1, c2));

    c1 = _mm_load_ps(cols[1].data);
    c2 = _mm_load_ps(right.cols[1].data);
    _mm_store_ps(cols[1].data, _mm_add_ps(c1, c2));

    c1 = _mm_load_ps(cols[2].data);
    c2 = _mm_load_ps(right.cols[2].data);
    _mm_store_ps(cols[2].data, _mm_add_ps(c1, c2));

    c1 = _mm_load_ps(cols[3].data);
    c2 = _mm_load_ps(right.cols[3].data);
    _mm_store_ps(cols[3].data, _mm_add_ps(c1, c2));

    return *this;
}

Mat4& Mat4::operator-=(const Mat4& right)
{
    __m128 c1, c2;

    c1 = _mm_load_ps(cols[0].data);
    c2 = _mm_load_ps(right.cols[0].data);
    _mm_store_ps(cols[0].data, _mm_sub_ps(c1, c2));

    c1 = _mm_load_ps(cols[1].data);
    c2 = _mm_load_ps(right.cols[1].data);
    _mm_store_ps(cols[1].data, _mm_sub_ps(c1, c2));

    c1 = _mm_load_ps(cols[2].data);
    c2 = _mm_load_ps(right.cols[2].data);
    _mm_store_ps(cols[2].data, _mm_sub_ps(c1, c2));

    c1 = _mm_load_ps(cols[3].data);
    c2 = _mm_load_ps(right.cols[3].data);
    _mm_store_ps(cols[3].data, _mm_sub_ps(c1, c2));

    return *this;
}

Mat4& Mat4::operator*=(const Mat4& right)
{
    cols[0] = right * cols[0];
    cols[1] = right * cols[1];
    cols[2] = right * cols[2];
    cols[3] = right * cols[3];

    return *this;
}

Mat4& Mat4::operator*=(f32 scalar)
{
    __m128 s = _mm_set1_ps(scalar);
    __m128 col;

    col = _mm_load_ps(cols[0].data);
    _mm_store_ps(cols[0].data, _mm_mul_ps(col, s));

    col = _mm_load_ps(cols[1].data);
    _mm_store_ps(cols[1].data, _mm_mul_ps(col, s));
    
    col = _mm_load_ps(cols[2].data);
    _mm_store_ps(cols[2].data, _mm_mul_ps(col, s));
    
    col = _mm_load_ps(cols[3].data);
    _mm_store_ps(cols[3].data, _mm_mul_ps(col, s));

    return *this;
}