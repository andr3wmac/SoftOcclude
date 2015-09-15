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
#ifndef __SSE_4_1_VEC2B_H__
#define __SSE_4_1_VEC2B_H__

#include "math/types.h"

#include "math/simd/generic/simdBaseTraits.h"
#include "math/simd/generic/simdVectorBase.h"

#include "math/simd/sse4_1/sse4_1_vec2d_b.h"

#include <smmintrin.h>
#include <iostream>
#include <limits>

class SSE41Vec2d;

template <>
struct SSE4_1SimdTraits<F64> : public BaseSimdTraits<F64>
{
    typedef F64 value_type;
    typedef SSE41Vec2d vec_type;
    typedef SSE41Vec2d_b bool_type;
    static const size_t width = 2;
    static const size_t bytesPerValue = 8;
    static const size_t registers = 1;
    static const size_t alignment = 16;
};

class SSE41Vec2d : public SimdVectorBase< SSE41Vec2d, F64>
{
public:

    SSE41Vec2d()
    {}

    inline SSE41Vec2d( F64 val ) : mValue( _mm_set1_pd( val ) )
    {
    }

    /*
    inline SSE41Vec2d( F64 v0, F64 v1, F64 v2, F64 v3 ) :
        mValue( _mm_setr_pd( v0, v1, v2, v3 ) )
    {
    }
    */
    
    inline SSE41Vec2d( const __m128d &rhs ) : mValue( rhs )
    {

    }

    inline SSE41Vec2d &operator=( const __m128d &rhs )
    {
        mValue = rhs;

        return *this;
    }

    inline operator __m128d() const
    {
        return mValue;
    }

    void LoadUnaligned( const F64 *src )
    {
        mValue =  _mm_loadu_pd( src );
    }

    void LoadAligned( const F64 *src )
    {
        mValue = _mm_load_pd( src );
    }

    void StoreUnaligned( F64 *dest ) const
    {
        _mm_storeu_pd( dest, mValue );
    }

    void StoreAligned( F64 *dest ) const
    {
        _mm_store_pd( dest, mValue );
    }

    void RotateOne( bool permute128 )
    {
        const S32 select = ( 1 ) | ( 2 << 2 ) | ( 3 << 4 ) | ( 0 << 6 );
        mValue = _mm_shuffle_pd( mValue, mValue, select );
    }

    inline SSE41Vec2d RoundToNearest() const
    {
        return _mm_round_pd( mValue, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC );
    }

private:

    __m128d mValue;
};

//
// Math
//

inline SSE41Vec2d operator+( const SSE41Vec2d &lhs, const SSE41Vec2d &rhs )
{
    return _mm_add_pd( lhs, rhs );
}

inline SSE41Vec2d operator-( const SSE41Vec2d &lhs, const SSE41Vec2d &rhs )
{
    return _mm_sub_pd( lhs, rhs );
}

inline SSE41Vec2d operator*( const SSE41Vec2d &lhs, const SSE41Vec2d &rhs )
{
    return _mm_mul_pd( lhs, rhs );
}

inline SSE41Vec2d operator/( const SSE41Vec2d &lhs, const SSE41Vec2d &rhs )
{
    return _mm_div_pd( lhs, rhs );
}

//
// Comparison
//

inline SSE41Vec2d_b operator== ( const SSE41Vec2d &lhs, const SSE41Vec2d &rhs )
{
    return _mm_cmpeq_pd( lhs, rhs );
}

inline SSE41Vec2d_b operator!= ( const SSE41Vec2d &lhs, const SSE41Vec2d &rhs )
{
    return _mm_cmpneq_pd( lhs, rhs );
}

inline SSE41Vec2d_b operator< ( const SSE41Vec2d &lhs, const SSE41Vec2d &rhs )
{
    return _mm_cmplt_pd( lhs, rhs );
}

inline SSE41Vec2d_b operator<= ( const SSE41Vec2d &lhs, const SSE41Vec2d &rhs )
{
    return _mm_cmple_pd( lhs, rhs );
}

inline SSE41Vec2d_b operator> ( const SSE41Vec2d &lhs, const SSE41Vec2d &rhs )
{
    return _mm_cmpgt_pd( lhs, rhs );
}

inline SSE41Vec2d_b operator>= ( const SSE41Vec2d &lhs, const SSE41Vec2d &rhs )
{
    return _mm_cmpge_pd( lhs, rhs );
}

//
// Special
//

inline SSE41Vec2d SIMD_Sqrt( const SSE41Vec2d &lhs )
{
    return _mm_sqrt_pd( lhs );
}

inline SSE41Vec2d SIMD_Rcp( const SSE41Vec2d &lhs )
{
    return 1.0 / lhs;
}


inline SSE41Vec2d SIMD_Select( const SSE41Vec2d_b &sel, const SSE41Vec2d &lhs, const SSE41Vec2d &rhs )
{
    return _mm_blendv_pd( rhs, lhs, sel );
}

//http://stackoverflow.com/questions/13219146/how-to-sum-m256-horizontally
inline F64 SIMD_Hadd( const SSE41Vec2d &lhs )
{
    __m128d tmp0 = _mm_hadd_pd( lhs, lhs );

    return _mm_cvtsd_f64( tmp0 );
}

inline SSE41Vec2d FMA_ADD( const SSE41Vec2d &mul1, const SSE41Vec2d &mul2, const SSE41Vec2d &add )
{
    return mul1 * mul2 + add;
}

inline SSE41Vec2d FMA_SUB( const SSE41Vec2d &mul1, const SSE41Vec2d &mul2, const SSE41Vec2d &sub )
{
    return mul1 * mul2 - sub;
}

#endif