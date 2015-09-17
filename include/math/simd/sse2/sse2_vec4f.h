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
#ifndef __SSE2_VEC4f_H__
#define __SSE2_VEC4f_H__

#include "math/types.h"

#include "math/simd/generic/simdBaseTraits.h"
#include "math/simd/generic/simdVectorBase.h"

#include "math/simd/sse2/sse2_vec4f_b.h"

#include <xmmintrin.h>
#include <iostream>
#include <limits>

class SSE2Vec4f;

template <>
struct SSE2SimdTraits<F32> : public BaseSimdTraits<F32>
{
    typedef F32 value_type;
    typedef SSE2Vec4f vec_type;
    typedef SSE2Vec4f_b bool_type;
    static const size_t width = 4;
    static const size_t bytesPerValue = 4;
    static const size_t registers = 1;
    static const size_t alignment = 16;
};

class SSE2Vec4f : public SimdVectorBase< SSE2Vec4f, F32>
{
public:

    SSE2Vec4f()
    {}

    inline SSE2Vec4f( F32 val ) : mValue( _mm_set1_ps( val ) )
    {
    }
    
    inline SSE2Vec4f( const __m128 &rhs ) : mValue( rhs )
    {

    }

    inline SSE2Vec4f(const __m128i &rhs) : mValue(_mm_cvtepi32_ps(rhs))
    {

    }

    inline SSE2Vec4f &operator=( const __m128 &rhs )
    {
        mValue = rhs;

        return *this;
    }

    inline operator __m128() const
    {
        return mValue;
    }

    void LoadUnaligned( const F32 *src )
    {
        mValue =  _mm_loadu_ps( src );
    }

    void LoadAligned( const F32 *src )
    {
        mValue = _mm_load_ps( src );
    }

    void StoreUnaligned( F32 *dest ) const
    {
        _mm_storeu_ps( dest, mValue );
    }

    void StoreAligned( F32 *dest ) const
    {
        _mm_store_ps( dest, mValue );
    }

    void RotateOne( bool permute128 )
    {
        const S32 select = ( 1 ) | ( 2 << 2 ) | ( 3 << 4 ) | ( 0 << 6 );
        mValue = _mm_shuffle_ps( mValue, mValue, select );
    }

    inline SSE2Vec4f RoundToNearest() const
    {
        return  _mm_cvtepi32_ps( _mm_cvtps_epi32( mValue ) );
    }

private:

    __m128 mValue;
};

//
// Math
//

inline SSE2Vec4f operator+( const SSE2Vec4f &lhs, const SSE2Vec4f &rhs )
{
    return _mm_add_ps( lhs, rhs );
}

inline SSE2Vec4f operator-( const SSE2Vec4f &lhs, const SSE2Vec4f &rhs )
{
    return _mm_sub_ps( lhs, rhs );
}

inline SSE2Vec4f operator*( const SSE2Vec4f &lhs, const SSE2Vec4f &rhs )
{
    return _mm_mul_ps( lhs, rhs );
}

inline SSE2Vec4f operator/( const SSE2Vec4f &lhs, const SSE2Vec4f &rhs )
{
    return _mm_div_ps( lhs, rhs );
}

//
// Comparison
//

inline SSE2Vec4f_b operator== ( const SSE2Vec4f &lhs, const SSE2Vec4f &rhs )
{
    return _mm_cmpeq_ps( lhs, rhs );
}

inline SSE2Vec4f_b operator!= ( const SSE2Vec4f &lhs, const SSE2Vec4f &rhs )
{
    return _mm_cmpneq_ps( lhs, rhs );
}

inline SSE2Vec4f_b operator< ( const SSE2Vec4f &lhs, const SSE2Vec4f &rhs )
{
    return _mm_cmplt_ps( lhs, rhs );
}

inline SSE2Vec4f_b operator<= ( const SSE2Vec4f &lhs, const SSE2Vec4f &rhs )
{
    return _mm_cmple_ps( lhs, rhs );
}

inline SSE2Vec4f_b operator> ( const SSE2Vec4f &lhs, const SSE2Vec4f &rhs )
{
    return _mm_cmpgt_ps( lhs, rhs );
}

inline SSE2Vec4f_b operator>= ( const SSE2Vec4f &lhs, const SSE2Vec4f &rhs )
{
    return _mm_cmpge_ps( lhs, rhs );
}

//
// Special
//

inline SSE2Vec4f SIMD_Sqrt( const SSE2Vec4f &lhs )
{
    return _mm_sqrt_ps( lhs );
}

inline SSE2Vec4f SIMD_Rcp( const SSE2Vec4f &lhs )
{
    return 1.0 / lhs;
    //return _mm_rcp_ps( lhs );
}

inline SSE2Vec4f SIMD_Select( const SSE2Vec4f_b &sel, const SSE2Vec4f &lhs, const SSE2Vec4f &rhs )
{
    __m128i mask = _mm_srai_epi32(_mm_castps_si128(sel), 31);
    __m128i b = _mm_and_si128(_mm_castps_si128(lhs), mask);
    __m128i a = _mm_andnot_si128(mask, _mm_castps_si128(rhs));

    return  _mm_castsi128_ps(_mm_or_si128(a, b));
}

//http://stackoverflow.com/questions/13219146/how-to-sum-m256-horizontally
inline F32 SIMD_Hadd( const SSE2Vec4f &lhs )
{
    const S32 select = (1) | (1 << 2) | (1 << 4) | (1 << 6);

    const __m128 x64 = _mm_add_ps(lhs, _mm_movehl_ps(lhs, lhs));
    const __m128 x32 = _mm_add_ss(x64, _mm_shuffle_ps(x64, x64, select));

    return _mm_cvtss_f32(x32);
}

inline SSE2Vec4f FMA_ADD( const SSE2Vec4f &mul1, const SSE2Vec4f &mul2, const SSE2Vec4f &add )
{
    return mul1 * mul2 + add;
}

inline SSE2Vec4f FMA_SUB( const SSE2Vec4f &mul1, const SSE2Vec4f &mul2, const SSE2Vec4f &sub )
{
    return mul1 * mul2 - sub;
}

#endif