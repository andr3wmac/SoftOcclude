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
#ifndef __SSE4_1_VEC2D_B_H__
#define __SSE4_1_VEC2D_B_H__

#include "math/types.h"
#include "math/config.h"

#include "math/simd/vectorize.h"
#include "math/simd/generic/simdVectorBoolBase.h"

#include <smmintrin.h>
#include <limits>

class SSE41Vec2d_b : public SimdVectorBoolBase< SSE41Vec2d_b >
{
public:

    SSE41Vec2d_b()
    {}

    FORCE_INLINE SSE41Vec2d_b( bool val ) :
        mValue( _mm_castsi128_pd( _mm_set1_epi32( -( int )val ) ) )
    {
    }

    /*
    FORCE_INLINE SSE41Vec2d_b( bool b0, bool b1, bool b2, bool b3 ) :
        mValue( _mm_castsi128_pd( _mm_setr_epi32( -( int )b0, -( int )b1, -( int )b2, -( int )b3 ) ) )
    {
    }
    */
    
    FORCE_INLINE SSE41Vec2d_b( const __m128d &rhs ) :
        mValue( rhs )
    {
    }

    FORCE_INLINE SSE41Vec2d_b(const SSE41Vec2d_b &rhs) :
        mValue(rhs.mValue)
    {
    }

    FORCE_INLINE SSE41Vec2d_b &operator=( const __m128d &rhs )
    {
        mValue = rhs;

        return *this;
    }

    FORCE_INLINE operator __m128d() const
    {
        return mValue;
    }

    //template< U32 rotate >
    FORCE_INLINE void LoadMask( U32 rotate, U64 mask )
    {
        const U32 rot0 = ( ( 0 + rotate ) & 0x1 );
        const U32 rot1 = ( ( 1 + rotate ) & 0x1 );

        // we use the value as bool, so it doesnt matter
        // if we epi32 this one 
        mValue = _mm_castsi128_pd( _mm_setr_epi32(
                                       -( S64 )( ( mask >> ( 0  + rot0 ) ) & 0x1 ),
                                       -( S64 )( ( mask >> ( 0  + rot0 ) ) & 0x1 ),
                                       -( S64 )( ( mask >> ( 2  + rot1 ) ) & 0x1 ),
                                       -( S64 )( ( mask >> ( 2  + rot1 ) ) & 0x1 )
                                    	       ) );
    }

    FORCE_INLINE U64 StoreMask() const
    {       
        return (U64)_mm_movemask_pd( mValue );
    }

    FORCE_INLINE void StoreAligned( F64 *dest ) const
    {
        _mm_store_pd( dest, mValue );
    }

    FORCE_INLINE void LoadAligned( const F64 *src )
    {
        mValue = _mm_load_pd( src );
    }

private:

    __m128d mValue;
};

FORCE_INLINE SSE41Vec2d_b operator&( const SSE41Vec2d_b &lhs, const SSE41Vec2d_b &rhs )
{
    return _mm_and_pd( lhs, rhs );
}

FORCE_INLINE SSE41Vec2d_b operator|( const SSE41Vec2d_b &lhs, const SSE41Vec2d_b &rhs )
{
    return _mm_or_pd( lhs, rhs );
}

FORCE_INLINE SSE41Vec2d_b operator^( const SSE41Vec2d_b &lhs, const SSE41Vec2d_b &rhs )
{
    return _mm_xor_pd( lhs, rhs );
}

FORCE_INLINE SSE41Vec2d_b operator==( const SSE41Vec2d_b &lhs, const SSE41Vec2d_b &rhs )
{
    return _mm_cmpeq_pd( lhs, rhs );
}

FORCE_INLINE SSE41Vec2d_b operator!=( const SSE41Vec2d_b &lhs, const SSE41Vec2d_b &rhs )
{
    return _mm_cmpneq_pd( lhs, rhs );
}

#endif