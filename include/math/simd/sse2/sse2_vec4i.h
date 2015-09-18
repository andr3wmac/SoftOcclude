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
#ifndef __SSE2_VEC4I_H__
#define __SSE2_VEC4I_H__

#include "math/types.h"
#include "math/config.h"

#include "math/simd/generic/simdBaseTraits.h"
#include "math/simd/generic/simdVectorBase.h"

#include "math/simd/SSE2/SSE2_vec4i_b.h"

#include <emmintrin.h>

#include <iostream>
#include <limits>

class SSE2Vec4i;

template <>
struct SSE2SimdTraits<S32> : public BaseSimdTraits<S32>
{
    typedef S32 value_type;
    typedef SSE2Vec4i vec_type;
    typedef SSE2Vec4i_b bool_type;
    static const size_t width = 4;
    static const size_t bytesPerValue = 4;
    static const size_t registers = 1;
    static const size_t alignment = 16;
};

class SSE2Vec4i : public SimdVectorBase< SSE2Vec4i, S32>
{
public:

    SSE2Vec4i()
    {}

    inline SSE2Vec4i(S32 val) : mValue(_mm_set1_epi32(val))
    {
    }

    inline SSE2Vec4i(const __m128i &rhs) : mValue(rhs)
    {

    }

    inline SSE2Vec4i(const SSE2Vec4i &rhs) : mValue(rhs.mValue)
    {

    }

    inline SSE2Vec4i(const __m128 &rhs) : mValue(_mm_cvtps_epi32(rhs) )
    {

    }

    inline SSE2Vec4i &operator=(const __m128i &rhs)
    {
        mValue = rhs;

        return *this;
    }

    inline SSE2Vec4i operator<<( S32 shift) const
    {
        return _mm_slli_epi32(mValue, shift);
    }

    inline SSE2Vec4i operator|(const __m128i &rhs) const
    {
        return _mm_or_si128(mValue, rhs);
    }

    inline SSE2Vec4i operator&(const __m128i &rhs) const
    {
        return _mm_and_si128(mValue, rhs);
    }

    inline operator __m128i() const
    {
        return mValue;
    }

    inline SSE2Vec4i Min(const SSE2Vec4i &rhs) const
    {
        return _mm_xor_si128(rhs, _mm_and_si128(_mm_cmplt_epi32(mValue, rhs), _mm_xor_si128(mValue, rhs)));
    }

    inline SSE2Vec4i Max(const SSE2Vec4i &rhs) const
    {
        return _mm_xor_si128(rhs, _mm_and_si128(_mm_cmpgt_epi32(mValue, rhs), _mm_xor_si128(mValue, rhs)));
    }

    inline bool IsEmpty() const
    {
        __m128i zero = _mm_setzero_si128();
        return _mm_movemask_epi8(_mm_cmpeq_epi32(mValue, zero)) == 0xFFFF;
    }

    void LoadAligned(S32 *dest)
    {
        mValue = _mm_load_si128((__m128i*)dest);
    }

    void StoreAligned(S32 *dest) const
    {
        _mm_store_si128( (__m128i*)dest, mValue);
    }

    template< U32 i >
    inline SSE2Vec4i BroadcastIndex() const
    {
        assert(i < 4);

        const S32 select = i | (i << 2) | (i << 4) | (i << 6);

        return _mm_shuffle_epi32(mValue, select);
    }

    template< U32 i >
    inline S32 ExtractIndex() const
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

inline SSE2Vec4i operator+(const SSE2Vec4i &lhs, const SSE2Vec4i &rhs)
{
    return _mm_add_epi32(lhs, rhs);
}

inline SSE2Vec4i operator-(const SSE2Vec4i &lhs, const SSE2Vec4i &rhs)
{
    return _mm_sub_epi32(lhs, rhs);
}

inline SSE2Vec4i operator*(const SSE2Vec4i &lhs, const SSE2Vec4i &rhs)
{
    //http://stackoverflow.com/questions/10500766/sse-multiplication-of-4-32-bit-integers
    __m128i tmp1 = _mm_mul_epu32(lhs, rhs);
    __m128i tmp2 = _mm_mul_epu32(_mm_srli_si128(lhs, 4), _mm_srli_si128(rhs, 4));
    return _mm_unpacklo_epi32(_mm_shuffle_epi32(tmp1, _MM_SHUFFLE(0, 0, 2, 0)), _mm_shuffle_epi32(tmp2, _MM_SHUFFLE(0, 0, 2, 0)));
}

//
// Comparison
//

inline SSE2Vec4i_b operator== (const SSE2Vec4i &lhs, const SSE2Vec4i &rhs)
{
    return _mm_cmpeq_epi32(lhs, rhs);
}

inline SSE2Vec4i_b operator< (const SSE2Vec4i &lhs, const SSE2Vec4i &rhs)
{
    return _mm_cmplt_epi32(lhs, rhs);
}

inline SSE2Vec4i_b operator> (const SSE2Vec4i &lhs, const SSE2Vec4i &rhs)
{
    return _mm_cmpgt_epi32(lhs, rhs);
}

inline SSE2Vec4i SIMD_Select(const SSE2Vec4i_b &sel, const SSE2Vec4i &lhs, const SSE2Vec4i &rhs)
{
    __m128i mask = _mm_srai_epi32(sel, 31);
    __m128i b = _mm_and_si128(lhs, mask);
    __m128i a = _mm_andnot_si128(mask,rhs);

    return  _mm_or_si128(a, b);
}


#endif