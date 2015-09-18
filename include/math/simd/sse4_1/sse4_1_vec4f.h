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
#ifndef __SSE4_1_VEC4f_H__
#define __SSE4_1_VEC4f_H__

#include "math/types.h"
#include "math/config.h"

#include "math/simd/generic/simdBaseTraits.h"
#include "math/simd/generic/simdVectorBase.h"

#include "math/simd/sse4_1/sse4_1_vec4f_b.h"

#include <smmintrin.h>
#include <iostream>
#include <limits>
#include <assert.h>

class SSE41Vec4f;

template <>
struct SSE4_1SimdTraits<F32> : public BaseSimdTraits<F32>
{
    typedef F32 value_type;
    typedef SSE41Vec4f vec_type;
    typedef SSE41Vec4f_b bool_type;
    static const size_t width = 4;
    static const size_t bytesPerValue = 4;
    static const size_t registers = 1;
    static const size_t alignment = 16;
};

class SSE41Vec4f : public SimdVectorBase< SSE41Vec4f, F32>
{
public:

    SSE41Vec4f()
    {}

    inline SSE41Vec4f( F32 val ) : mValue( _mm_set1_ps( val ) )
    {
    }

    /*
    inline SSE41Vec4f( F32 v0, F32 v1, F32 v2, F32 v3 ) :
        mValue( _mm_setr_ps( v0, v1, v2, v3 ) )
    {
    }
    */
    
    inline SSE41Vec4f( const __m128 &rhs ) : mValue( rhs )
    {

    }

    inline SSE41Vec4f(const SSE41Vec4f &rhs) : mValue(rhs.mValue)
    {

    }

    inline SSE41Vec4f(const __m128i &rhs) : mValue(_mm_cvtepi32_ps(rhs))
    {

    }

    inline SSE41Vec4f &operator=( const __m128 &rhs )
    {
        mValue = rhs;

        return *this;
    }

    inline operator __m128() const
    {
        return mValue;
    }

    inline void LoadUnaligned( const F32 *src )
    {
        mValue =  _mm_loadu_ps( src );
    }

    inline void LoadAligned( const F32 *src )
    {
        mValue = _mm_load_ps( src );
    }

    inline void StoreUnaligned( F32 *dest ) const
    {
        _mm_storeu_ps( dest, mValue );
    }

    inline void StoreAligned( F32 *dest ) const
    {
        _mm_store_ps( dest, mValue );
    }

    inline void RotateOne( bool permute128 )
    {
        const S32 select = ( 1 ) | ( 2 << 2 ) | ( 3 << 4 ) | ( 0 << 6 );
        mValue = _mm_shuffle_ps( mValue, mValue, select );
    }

    inline SSE41Vec4f RoundToNearest() const
    {
        return _mm_round_ps( mValue, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC );
    }

    template< U32 i >
    inline SSE41Vec4f BroadcastIndex() const
    {
        assert(i < 4);

        const S32 select = i | (i << 2) | (i << 4) | (i << 6);

        return _mm_shuffle_ps(mValue, mValue, select);
    }

    template< U32 i >
    inline F32 ExtractIndex() const
    {
        assert(i < 4);

        const S32 select = i | (i << 2) | (i << 4) | (i << 6);

        return _mm_cvtss_f32(_mm_shuffle_ps(mValue, mValue, select));
    }
    

private:

    __m128 mValue;
};

//
// Math
//

inline SSE41Vec4f operator+( const SSE41Vec4f &lhs, const SSE41Vec4f &rhs )
{
    return _mm_add_ps( lhs, rhs );
}

inline SSE41Vec4f operator-( const SSE41Vec4f &lhs, const SSE41Vec4f &rhs )
{
    return _mm_sub_ps( lhs, rhs );
}

inline SSE41Vec4f operator*( const SSE41Vec4f &lhs, const SSE41Vec4f &rhs )
{
    return _mm_mul_ps( lhs, rhs );
}

inline SSE41Vec4f operator/( const SSE41Vec4f &lhs, const SSE41Vec4f &rhs )
{
    return _mm_div_ps( lhs, rhs );
}

//
// Comparison
//

inline SSE41Vec4f_b operator== ( const SSE41Vec4f &lhs, const SSE41Vec4f &rhs )
{
    return _mm_cmpeq_ps( lhs, rhs );
}

inline SSE41Vec4f_b operator!= ( const SSE41Vec4f &lhs, const SSE41Vec4f &rhs )
{
    return _mm_cmpneq_ps( lhs, rhs );
}

inline SSE41Vec4f_b operator< ( const SSE41Vec4f &lhs, const SSE41Vec4f &rhs )
{
    return _mm_cmplt_ps( lhs, rhs );
}

inline SSE41Vec4f_b operator<= ( const SSE41Vec4f &lhs, const SSE41Vec4f &rhs )
{
    return _mm_cmple_ps( lhs, rhs );
}

inline SSE41Vec4f_b operator> ( const SSE41Vec4f &lhs, const SSE41Vec4f &rhs )
{
    return _mm_cmpgt_ps( lhs, rhs );
}

inline SSE41Vec4f_b operator>= ( const SSE41Vec4f &lhs, const SSE41Vec4f &rhs )
{
    return _mm_cmpge_ps( lhs, rhs );
}

//
// Special
//

inline SSE41Vec4f SIMD_Sqrt( const SSE41Vec4f &lhs )
{
    return _mm_sqrt_ps( lhs );
}

inline SSE41Vec4f SIMD_Rcp( const SSE41Vec4f &lhs )
{
    return 1.0 / lhs;
    //return _mm_rcp_ps( lhs );
}

inline SSE41Vec4f SIMD_Select( const SSE41Vec4f_b &sel, const SSE41Vec4f &lhs, const SSE41Vec4f &rhs )
{
    return _mm_blendv_ps( rhs, lhs, sel );
}

//http://stackoverflow.com/questions/13219146/how-to-sum-m256-horizontally
inline F32 SIMD_Hadd( const SSE41Vec4f &lhs )
{
    __m128 tmp0 = _mm_hadd_ps( lhs, lhs );
    __m128 tmp1 = _mm_hadd_ps( tmp0, tmp0 );

    return _mm_cvtss_f32( tmp1 );
}

inline SSE41Vec4f FMA_ADD( const SSE41Vec4f &mul1, const SSE41Vec4f &mul2, const SSE41Vec4f &add )
{
    return mul1 * mul2 + add;
}

inline SSE41Vec4f FMA_SUB( const SSE41Vec4f &mul1, const SSE41Vec4f &mul2, const SSE41Vec4f &sub )
{
    return mul1 * mul2 - sub;
}

#endif