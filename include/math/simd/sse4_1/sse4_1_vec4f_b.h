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
#ifndef __SSE41_VEC4B_H__
#define __SSE41_VEC4B_H__

#include "math/types.h"
#include "math/config.h"

#include "math/simd/vectorize.h"
#include "math/simd/generic/simdVectorBoolBase.h"

#include <smmintrin.h>
#include <limits>

class SSE41Vec4f_b : public SimdVectorBoolBase< SSE41Vec4f_b >
{
public:

    SSE41Vec4f_b()
    {}

    FORCE_INLINE SSE41Vec4f_b( bool val ) :
        mValue( _mm_castsi128_ps( _mm_set1_epi32( -( int )val ) ) )
    {
    }

    /*
    FORCE_INLINE SSE41Vec4f_b( bool b0, bool b1, bool b2, bool b3 ) :
        mValue( _mm_castsi128_ps( _mm_setr_epi32( -( int )b0, -( int )b1, -( int )b2, -( int )b3 ) ) )
    {
    }
    */
    
    FORCE_INLINE SSE41Vec4f_b(const __m128i &rhs) : mValue(_mm_cvtepi32_ps(rhs))
    {

    }

    FORCE_INLINE SSE41Vec4f_b( const __m128 &rhs ) :
        mValue( rhs )
    {
    }

    FORCE_INLINE SSE41Vec4f_b(const SSE41Vec4f_b &rhs) :
        mValue(rhs.mValue)
    {
    }

    FORCE_INLINE SSE41Vec4f_b &operator=( const __m128 &rhs )
    {
        mValue = rhs;

        return *this;
    }

    FORCE_INLINE operator __m128() const
    {
        return mValue;
    }

    //template< U32 rotate >
    FORCE_INLINE void LoadMask( U32 rotate, U64 mask )
    {
        const U32 rot0 = ( ( 0 + rotate ) & 0x03 );
        const U32 rot1 = ( ( 1 + rotate ) & 0x03 );
        const U32 rot2 = ( ( 2 + rotate ) & 0x03 );
        const U32 rot3 = ( ( 3 + rotate ) & 0x03 );

        mValue = _mm_castsi128_ps( _mm_setr_epi32(
                                       -( S32 )( ( mask >> ( 0  + rot0 ) ) & 0x1 ),
                                       -( S32 )( ( mask >> ( 4  + rot1 ) ) & 0x1 ),
                                       -( S32 )( ( mask >> ( 8  + rot2 ) )  & 0x1 ),
                                       -( S32 )( ( mask >> ( 12 + rot3 ) ) & 0x1 )
                                   ) );
    }
    
    FORCE_INLINE U64 StoreMask() const
    {       
        return (U64)_mm_movemask_ps( mValue );
    }

    FORCE_INLINE void StoreAligned( F32 *dest ) const
    {
        _mm_store_ps( dest, mValue );
    }

    FORCE_INLINE void LoadAligned( const F32 *src )
    {
        mValue = _mm_load_ps( src );
    }

private:

    __m128 mValue;
};

FORCE_INLINE SSE41Vec4f_b operator&( const SSE41Vec4f_b &lhs, const SSE41Vec4f_b &rhs )
{
    return _mm_and_ps( lhs, rhs );
}

FORCE_INLINE SSE41Vec4f_b operator|( const SSE41Vec4f_b &lhs, const SSE41Vec4f_b &rhs )
{
    return _mm_or_ps( lhs, rhs );
}

FORCE_INLINE SSE41Vec4f_b operator^( const SSE41Vec4f_b &lhs, const SSE41Vec4f_b &rhs )
{
    return _mm_xor_ps( lhs, rhs );
}

FORCE_INLINE SSE41Vec4f_b operator==( const SSE41Vec4f_b &lhs, const SSE41Vec4f_b &rhs )
{
    return _mm_cmpeq_ps( lhs, rhs );
}

FORCE_INLINE SSE41Vec4f_b operator!=( const SSE41Vec4f_b &lhs, const SSE41Vec4f_b &rhs )
{
    return _mm_cmpneq_ps( lhs, rhs );
}

FORCE_INLINE bool SIMD_Hadd( const SSE41Vec4f_b &lhs )
{
    __m128 tmp0 = _mm_hadd_ps( lhs, lhs );
    __m128 tmp1 = _mm_hadd_ps( tmp0, tmp0 );

    return (bool)_mm_cvtss_si32( tmp1 );
}

#endif