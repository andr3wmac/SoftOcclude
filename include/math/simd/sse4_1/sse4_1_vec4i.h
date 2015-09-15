#pragma once
#ifndef __SSE4_1_VEC4I_H__
#define __SSE4_1_VEC4I_H__

#include "math/types.h"

#include "math/simd/generic/simdBaseTraits.h"
#include "math/simd/generic/simdVectorBase.h"

#include "math/simd/sse4_1/sse4_1_vec4i_b.h"

#include <smmintrin.h>

#include <iostream>
#include <limits>

class SSE41Vec4i;

template <>
struct SSE4_1SimdTraits<S32> : public BaseSimdTraits<S32>
{
    typedef S32 value_type;
    typedef SSE41Vec4i vec_type;
    typedef SSE41Vec4i_b bool_type;
    static const size_t width = 4;
    static const size_t bytesPerValue = 4;
    static const size_t registers = 1;
    static const size_t alignment = 16;
};

class SSE41Vec4i : public SimdVectorBase< SSE41Vec4i, S32>
{
public:

    SSE41Vec4i()
    {}

    inline SSE41Vec4i(S32 val) : mValue(_mm_set1_epi32(val))
    {
    }

    inline SSE41Vec4i(const __m128i &rhs) : mValue(rhs)
    {

    }

    inline SSE41Vec4i(const __m128 &rhs) : mValue(_mm_cvtps_epi32(rhs) )
    {

    }

    inline SSE41Vec4i &operator=(const __m128i &rhs)
    {
        mValue = rhs;

        return *this;
    }

    inline SSE41Vec4i operator<<( S32 shift)
    {
        return _mm_slli_epi32(mValue, shift);
    }

    inline SSE41Vec4i operator|(const __m128i &rhs)
    {
        return _mm_or_si128(mValue, rhs);
    }

    inline SSE41Vec4i operator&(const __m128i &rhs)
    {
        return _mm_and_si128(mValue, rhs);
    }

    inline operator __m128i() const
    {
        return mValue;
    }

    inline SSE41Vec4i Min(const SSE41Vec4i &rhs)
    {
        return _mm_min_epi32(mValue, rhs);
    }

    inline SSE41Vec4i Max(const SSE41Vec4i &rhs)
    {
        return _mm_max_epi32(mValue, rhs);
    }

    inline bool IsEmpty()
    {
        return _mm_test_all_zeros(mValue, mValue) == 1;
    }

    void LoadAligned(S32 *dest)
    {
        mValue = _mm_load_si128((__m128i*)dest);
    }

    void StoreAligned(S32 *dest) const
    {
        _mm_store_si128( (__m128i*)dest, mValue);
    }

private:

    __m128i mValue;
};

//
// Math
//

inline SSE41Vec4i operator+(const SSE41Vec4i &lhs, const SSE41Vec4i &rhs)
{
    return _mm_add_epi32(lhs, rhs);
}

inline SSE41Vec4i operator-(const SSE41Vec4i &lhs, const SSE41Vec4i &rhs)
{
    return _mm_sub_epi32(lhs, rhs);
}

inline SSE41Vec4i operator*(const SSE41Vec4i &lhs, const SSE41Vec4i &rhs)
{
    return _mm_mullo_epi32(lhs, rhs);
}

//
// Comparison
//

inline SSE41Vec4i_b operator== (const SSE41Vec4i &lhs, const SSE41Vec4i &rhs)
{
    return _mm_cmpeq_epi32(lhs, rhs);
}

inline SSE41Vec4i_b operator< (const SSE41Vec4i &lhs, const SSE41Vec4i &rhs)
{
    return _mm_cmplt_epi32(lhs, rhs);
}

inline SSE41Vec4i_b operator> (const SSE41Vec4i &lhs, const SSE41Vec4i &rhs)
{
    return _mm_cmpgt_epi32(lhs, rhs);
}

#endif