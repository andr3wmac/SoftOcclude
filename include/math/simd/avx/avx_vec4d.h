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
#ifndef __AVXVEC4D_H__
#define __AVXVEC4D_H__

#include "math/types.h"
#include "math/config.h"

#include "math/simd/generic/simdBaseTraits.h"
#include "math/simd/generic/simdVectorBase.h"
#include "math/simd/avx/avx_vec4d_b.h"

#include <immintrin.h>
#include <iostream>
#include <limits>

class AvxVec4d;

template <>
struct AvxSimdTraits<F64> : public BaseSimdTraits<F64>
{  
    typedef AvxVec4d vec_type;
    typedef AvxVec4d_b bool_type;
    static const size_t width = 4;
    static const size_t bytesPerValue = 8;
    static const size_t registers = 2;
    static const size_t alignment = 32;
};

class AvxVec4d : public SimdVectorBase< AvxVec4d, F64>
{
public:

    AvxVec4d()
    {}

    FORCE_INLINE AvxVec4d( F64 val ) : mValue( _mm256_set1_pd( val ) )
    {
    }

    /*
    FORCE_INLINE AvxVec4d( F64 v0, F64 v1, F64 v2, F64 v3 ) :
        mValue( _mm256_setr_pd( v0, v1, v2, v3 ) )
    {
    }
    */

    FORCE_INLINE AvxVec4d( const __m256d &rhs ) : mValue( rhs )
    {

    }

    FORCE_INLINE AvxVec4d(const AvxVec4d &rhs) : mValue(rhs.mValue)
    {

    }

    FORCE_INLINE AvxVec4d &operator=( const __m256d &rhs )
    {
        mValue = rhs;

        return *this;
    }

    FORCE_INLINE operator __m256d() const
    {
        return mValue;
    }

    FORCE_INLINE void LoadUnaligned( const F64 *src )
    {
        mValue =  _mm256_loadu_pd( src );
    }

    FORCE_INLINE void LoadAligned( const F64 *src )
    {
        mValue = _mm256_load_pd( src );
    }

    FORCE_INLINE void StoreUnaligned( F64 *dest ) const
    {
        _mm256_storeu_pd( dest, mValue );
    }

    FORCE_INLINE void StoreAligned( F64 *dest ) const
    {
        _mm256_store_pd( dest, mValue );
    }

    FORCE_INLINE void RotateOne( bool permute128 )
    {
        //                 64:127     0:63        255:192     191:128  
        const S32 select = ( 1 ) | ( 0 << 1 ) | ( 1 << 2 ) | ( 0 << 3 );
        AvxVec4d permute = _mm256_permute_pd( mValue, select );

        if ( permute128 )
        {
            mValue = _mm256_permute2f128_pd( permute, permute, 1 );
        }
        else
        {
            mValue = permute;
        }
    }
    
    FORCE_INLINE AvxVec4d RoundToNearest() const
    {
        return _mm256_round_pd( mValue, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC );
    }

private:

    __m256d mValue;
};

//
// Math
//

FORCE_INLINE AvxVec4d operator+( const AvxVec4d &lhs, const AvxVec4d &rhs )
{
    return _mm256_add_pd( lhs, rhs );
}

FORCE_INLINE AvxVec4d operator-( const AvxVec4d &lhs, const AvxVec4d &rhs )
{
    return _mm256_sub_pd( lhs, rhs );
}

FORCE_INLINE AvxVec4d operator*( const AvxVec4d &lhs, const AvxVec4d &rhs )
{
    return _mm256_mul_pd( lhs, rhs );
}

FORCE_INLINE AvxVec4d operator/( const AvxVec4d &lhs, const AvxVec4d &rhs )
{
    return _mm256_div_pd( lhs, rhs );
}

//
// Comparison
//

FORCE_INLINE AvxVec4d_b operator== ( const AvxVec4d &lhs, const AvxVec4d &rhs )
{
    return _mm256_cmp_pd( lhs, rhs, 0 );
}

FORCE_INLINE AvxVec4d_b operator!= ( const AvxVec4d &lhs, const AvxVec4d &rhs )
{
    return _mm256_cmp_pd( lhs, rhs, 12 );
}

FORCE_INLINE AvxVec4d_b operator< ( const AvxVec4d &lhs, const AvxVec4d &rhs )
{
    return _mm256_cmp_pd( lhs, rhs, 17 );
}

FORCE_INLINE AvxVec4d_b operator<= ( const AvxVec4d &lhs, const AvxVec4d &rhs )
{
    return _mm256_cmp_pd( lhs, rhs, 18 );
}

FORCE_INLINE AvxVec4d_b operator> ( const AvxVec4d &lhs, const AvxVec4d &rhs )
{
    return _mm256_cmp_pd( lhs, rhs, 30 );
}

FORCE_INLINE AvxVec4d_b operator>= ( const AvxVec4d &lhs, const AvxVec4d &rhs )
{
    return _mm256_cmp_pd( lhs, rhs, 29 );
}

//
// Special
//

FORCE_INLINE AvxVec4d SIMD_Sqrt( const AvxVec4d &lhs )
{
    return _mm256_sqrt_pd( lhs );
}

FORCE_INLINE AvxVec4d SIMD_Rcp( const AvxVec4d &lhs )
{
    return ( 1.0 / lhs ); 
}

FORCE_INLINE AvxVec4d SIMD_Select( const AvxVec4d_b &sel, const AvxVec4d &lhs, const AvxVec4d &rhs )
{
    return _mm256_blendv_pd( rhs, lhs, sel );
}

FORCE_INLINE F64 SIMD_Hadd( const AvxVec4d &lhs )
{
    const __m128d x128 = _mm_add_pd(_mm256_extractf128_pd(lhs, 1), _mm256_castpd256_pd128(lhs));
    const __m128d x64 = _mm_add_sd( x128, _mm_shuffle_pd(x128, x128, 0x3 ) );
    return _mm_cvtsd_f64(x64);
}

FORCE_INLINE AvxVec4d FMA_ADD( const AvxVec4d &mul1, const AvxVec4d &mul2, const AvxVec4d &add )
{
#if SIMD_INSTRUCTION_SET >= 7
    return _mm256_fmadd_pd( mul1, mul2, add );
#else
    return ( mul1 * mul2 ) + add;
#endif
}

FORCE_INLINE AvxVec4d FMA_SUB( const AvxVec4d &mul1, const AvxVec4d &mul2, const AvxVec4d &sub )
{
#if SIMD_INSTRUCTION_SET >= 7
    return _mm256_fmsub_pd( mul1, mul2, sub );
#else
    return ( mul1 * mul2 ) - sub;
#endif
}

#endif