/**
* @cond ___LICENSE___
*
* Copyright (c) 2014 Koen Visscher, Paul Visscher and individual contributors.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* @endcond
*/

#pragma once
#ifndef __SSE4_1_VEC4I_H__
#define __SSE4_1_VEC4I_H__

#include "math/types.h"
#include "math/config.h"

#include "math/simd/generic/simdBaseTraits.h"
#include "math/simd/generic/simdVectorBase.h"

#include "math/simd/sse4_1/sse4_1_vec4i_b.h"

#include <smmintrin.h>

#include <iostream>
#include <limits>
#include <assert.h>

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

    inline SSE41Vec4i(const SSE41Vec4i &rhs) : mValue(rhs.mValue)
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

    inline SSE41Vec4i operator<<( S32 shift) const
    {
        return _mm_slli_epi32(mValue, shift);
    }

    inline SSE41Vec4i operator|(const __m128i &rhs) const
    {
        return _mm_or_si128(mValue, rhs);
    }

    inline SSE41Vec4i operator&(const __m128i &rhs) const
    {
        return _mm_and_si128(mValue, rhs);
    }

    inline operator __m128i() const
    {
        return mValue;
    }

    inline SSE41Vec4i Min(const SSE41Vec4i &rhs) const
    {
        return _mm_min_epi32(mValue, rhs);
    }

    inline SSE41Vec4i Max(const SSE41Vec4i &rhs) const
    {
        return _mm_max_epi32(mValue, rhs);
    }

    inline bool IsEmpty() const
    {
        return _mm_test_all_zeros(mValue, mValue) == 1;
    }

    FORCE_INLINE void LoadAligned(S32 *dest)
    {
        mValue = _mm_load_si128((__m128i*)dest);
    }

    FORCE_INLINE void StoreAligned(S32 *dest) const
    {
        _mm_store_si128( (__m128i*)dest, mValue);
    }

    template< U32 i >
    FORCE_INLINE SSE41Vec4i BroadcastIndex() const
    {
        assert(i < 4);

        const S32 select = i | (i << 2) | (i << 4) | (i << 6);

        return _mm_shuffle_epi32(mValue, select);
    }

    template< U32 i >
    FORCE_INLINE S32 ExtractIndex() const
    {
        assert(i < 4);

        const S32 select = i | (i << 2) | (i << 4) | (i << 6);

        return _mm_cvtsi128_si32(_mm_shuffle_epi32(mValue, select));
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

inline SSE41Vec4i SIMD_Select(const SSE41Vec4i_b &sel, const SSE41Vec4i &lhs, const SSE41Vec4i &rhs)
{
    return _mm_castps_si128(_mm_blendv_ps(_mm_castsi128_ps(rhs), _mm_castsi128_ps(lhs), _mm_castsi128_ps(sel) ));
}

#endif