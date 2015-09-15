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
#ifndef __AVX2_VEC8f_H__
#define __AVX2_VEC8f_H__

#include "math/types.h"

#include "math/simd/generic/simdBaseTraits.h"
#include "math/simd/generic/simdVectorBase.h"

#include "math/simd/avx/avx_vec8f_b.h"

#include <immintrin.h>
#include <iostream>
#include <limits>


class AvxVec8f;

template <>
struct AvxSimdTraits<F32> : public BaseSimdTraits<F32>
{
    
    typedef AvxVec8f vec_type;
    typedef AvxVec8f_b bool_type;
    static const size_t width = 8;
    static const size_t bytesPerValue = 4;
    static const size_t registers = 2;
    static const size_t alignment = 32;
};

class AvxVec8f : public SimdVectorBase< AvxVec8f, F32>
{
public:

    AvxVec8f()
    {}

    inline AvxVec8f( F32 val ) : mValue( _mm256_set1_ps( val ) )
    {
    }

    /*
    inline AvxVec8f( F32 v0, F32 v1, F32 v2, F32 v3,
                     F32 v4, F32 v5, F32 v6, F32 v7 ) :
        mValue( _mm256_setr_ps( v0, v1, v2, v3, v4, v5, v6, v7 ) )
    {
    }
    */

    inline AvxVec8f( const __m256 &rhs ) : mValue( rhs )
    {

    }

    inline AvxVec8f &operator=( const __m256 &rhs )
    {
        mValue = rhs;

        return *this;
    }

    inline operator __m256() const
    {
        return mValue;
    }

    inline void LoadUnaligned( const F32 *src )
    {
        mValue =  _mm256_loadu_ps( src );
    }

    inline void LoadAligned( const F32 *src )
    {
        mValue = _mm256_load_ps( src );
    }

    inline void StoreUnaligned( F32 *dest ) const
    {
        _mm256_storeu_ps( dest, mValue );
    }

    inline void StoreAligned( F32 *dest ) const
    {
        _mm256_store_ps( dest, mValue );
    }

    inline void RotateOne( bool permute128 )
    {
        const S32 select = ( 1 ) | ( 2 << 2 ) | ( 3 << 4 ) | ( 0 << 6 );
        AvxVec8f permute = _mm256_permute_ps( mValue, select );

        if ( permute128 )
        {
            mValue = _mm256_permute2f128_ps( permute, permute, 1 );
        }
        else
        {
            mValue = permute;
        }
    }
    
    inline AvxVec8f RoundToNearest() const
    {
        return _mm256_round_ps( mValue, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC );
        
        //return _mm256_round_ps( mValue + AvxVec8f( 0.5 ), _MM_FROUND_TO_ZERO  );
    }

private:

    __m256 mValue;
};

//
// Math
//

inline AvxVec8f operator+( const AvxVec8f &lhs, const AvxVec8f &rhs )
{
    return _mm256_add_ps( lhs, rhs );
}

inline AvxVec8f operator-( const AvxVec8f &lhs, const AvxVec8f &rhs )
{
    return _mm256_sub_ps( lhs, rhs );
}

inline AvxVec8f operator*( const AvxVec8f &lhs, const AvxVec8f &rhs )
{
    return _mm256_mul_ps( lhs, rhs );
}

inline AvxVec8f operator/( const AvxVec8f &lhs, const AvxVec8f &rhs )
{
    return _mm256_div_ps( lhs, rhs );
}

//
// Comparison
//

// O: Ordered // NaN and 1.0 gives false
// U: Unordered // NaN and 1.0 gives false
// S: Signal NaN exceptions
// Q: Do not throw NaN exceptions

inline AvxVec8f_b operator== ( const AvxVec8f &lhs, const AvxVec8f &rhs )
{
    return _mm256_cmp_ps( lhs, rhs, 0 );
}

inline AvxVec8f_b operator!= ( const AvxVec8f &lhs, const AvxVec8f &rhs )
{
    return _mm256_cmp_ps( lhs, rhs, 12 );
}

inline AvxVec8f_b operator< ( const AvxVec8f &lhs, const AvxVec8f &rhs )
{
    return _mm256_cmp_ps( lhs, rhs, 17 );
}

inline AvxVec8f_b operator<= ( const AvxVec8f &lhs, const AvxVec8f &rhs )
{
    return _mm256_cmp_ps( lhs, rhs, 18 );
}

inline AvxVec8f_b operator> ( const AvxVec8f &lhs, const AvxVec8f &rhs )
{
    return _mm256_cmp_ps( lhs, rhs, 30 );
}

inline AvxVec8f_b operator>= ( const AvxVec8f &lhs, const AvxVec8f &rhs )
{
    return _mm256_cmp_ps( lhs, rhs, 29 );
}

//
// Special
//

inline AvxVec8f SIMD_Sqrt( const AvxVec8f &lhs )
{
    return _mm256_sqrt_ps( lhs );
}

inline AvxVec8f SIMD_Rcp( const AvxVec8f &lhs )
{
    return 1.0 / lhs;
    //return _mm256_rcp_ps( lhs );
}

inline AvxVec8f SIMD_Select( const AvxVec8f_b &sel, const AvxVec8f &lhs, const AvxVec8f &rhs )
{
    return _mm256_blendv_ps( rhs, lhs, sel );
}

inline F32 SIMD_Hadd( const AvxVec8f &lhs )
{
    const __m128 x128 = _mm_add_ps(_mm256_extractf128_ps(lhs, 1), _mm256_castps256_ps128(lhs));
    const __m128 x64 = _mm_add_ps(x128, _mm_movehl_ps(x128, x128));
    const __m128 x32 = _mm_add_ss(x64, _mm_shuffle_ps(x64, x64, 0x55));
    return _mm_cvtss_f32(x32);
}

inline AvxVec8f FMA_ADD( const AvxVec8f &mul1, const AvxVec8f &mul2, const AvxVec8f &add )
{
#if SIMD_INSTRUCTION_SET >= 7
    return _mm256_fmadd_ps( mul1, mul2, add );
#else
    return ( mul1 * mul2 ) + add;
#endif
}

inline AvxVec8f FMA_SUB( const AvxVec8f &mul1, const AvxVec8f &mul2, const AvxVec8f &sub )
{
#if SIMD_INSTRUCTION_SET >= 7
    return _mm256_fmsub_ps( mul1, mul2, sub );
#else
    return ( mul1 * mul2 ) - sub;
#endif
}

#endif