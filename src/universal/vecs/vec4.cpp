#include "vec4.h"

#include <cmath>
#include <xmmintrin.h>
#include "../basic_types.h"
#include "../math.h"

inline f32 Vec4::SqrLength() const
{
    return x * x + y * y + z * z + w * w;
}

inline f32 Vec4::Length() const
{
    return sqrtf(x * x + y * y + z * z + w * w);
}

Vec4 Vec4::Unit() const
{
    return *this / Length();
}

Vec4& Vec4::Normalize()
{
    *this /= Length();
    return *this;
}

Vec4 Vec4::operator+(const Vec4& right) const
{
    Vec4 v;

    __m128 myVec = _mm_load_ps(data);
    __m128 rightVec = _mm_load_ps(right.data);

    __m128 res = _mm_add_ps(myVec, rightVec);
    _mm_store_ps(v.data, res);

    return v;
}

Vec4 Vec4::operator-(const Vec4& right) const
{
    Vec4 v;

    __m128 myVec = _mm_load_ps(data);
    __m128 rightVec = _mm_load_ps(right.data);

    __m128 res = _mm_sub_ps(myVec, rightVec);
    _mm_store_ps(v.data, res);

    return v;
}

Vec4 Vec4::operator*(f32 scalar) const
{
    Vec4 v;

    __m128 myVec = _mm_load_ps(data);
    __m128 sVec = _mm_set1_ps(scalar);

    __m128 res = _mm_mul_ps(myVec, sVec);
    _mm_store_ps(v.data, res);

    return v;
}

Vec4 Vec4::operator/(f32 scalar) const
{
    Vec4 v;

    __m128 myVec = _mm_load_ps(data);
    __m128 sVec = _mm_set1_ps(scalar);

    __m128 res = _mm_div_ps(myVec, sVec);
    _mm_store_ps(v.data, res);

    return v;
}

Vec4& Vec4::operator+=(const Vec4& right)
{
    __m128 myVec = _mm_load_ps(data);
    __m128 rightVec = _mm_load_ps(right.data);

    __m128 res = _mm_add_ps(myVec, rightVec);
    _mm_store_ps(data, res);

    return *this;
}

Vec4& Vec4::operator-=(const Vec4& right)
{
    __m128 myVec = _mm_load_ps(data);
    __m128 rightVec = _mm_load_ps(right.data);

    __m128 res = _mm_sub_ps(myVec, rightVec);
    _mm_store_ps(data, res);

    return *this;
}

Vec4& Vec4::operator*=(f32 scalar)
{
    __m128 myVec = _mm_load_ps(data);
    __m128 sVec = _mm_set1_ps(scalar);

    __m128 res = _mm_mul_ps(myVec, sVec);
    _mm_store_ps(data, res);

    return *this;
}

Vec4& Vec4::operator/=(f32 scalar)
{
    __m128 myVec = _mm_load_ps(data);
    __m128 sVec = _mm_set1_ps(scalar);

    __m128 res = _mm_div_ps(myVec, sVec);
    _mm_store_ps(data, res);

    return *this;
}

inline bool Vec4::operator==(const Vec4& right) const
{
    return x == right.x && y == right.y && z == right.z && w == right.w;
}

inline bool Vec4::operator!=(const Vec4& right) const
{
    return x != right.x || y != right.y || z != right.z || w != right.w;
}

f32 Dot(const Vec4& v1, const Vec4 v2)
{
    __m128 vec1 = _mm_load_ps(v1.data);
    __m128 vec2 = _mm_load_ps(v2.data);

    __m128 res = _mm_mul_ps(vec1, vec2);
    f32 data[4];
    _mm_store_ps(data, res);

    return data[0] + data[1] + data[2] + data[4];
}

inline f32 SqrDistance(const Vec4& v1, const Vec4& v2)
{
    return (v1 - v2).SqrLength();
}

inline f32 Distance(const Vec4& v1, const Vec4& v2)
{
    return (v1 - v2).Length();
}

Vec4 Lerp(const Vec4& a, const Vec4& b, f32 t)
{
    t = Clamp(t, 0.0f, 1.0f);
 
    Vec4 v;

    __m128 aVec = _mm_load_ps(a.data);
    __m128 tVec = _mm_set1_ps(t);

    __m128 bVec = _mm_load_ps(b.data);
    __m128 _1_tVec = _mm_set1_ps(1.0f - t);

    __m128 res = _mm_add_ps(
        _mm_mul_ps(aVec, tVec),
        _mm_mul_ps(bVec, _1_tVec)
    );
    _mm_store_ps(v.data, res);

    return v;
}