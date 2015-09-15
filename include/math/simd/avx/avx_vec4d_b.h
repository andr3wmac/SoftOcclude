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
#ifndef __AVX_VEC4D_B_H__
#define __AVX_VEC4D_B_H__
#include "math/types.h"

#include "math/scalar/mathf.h"

#include "math/simd/vectorize.h"
#include "math/simd/generic/simdVectorBoolBase.h"

#include <immintrin.h>
#include <iostream>
#include <limits>

class AvxVec4d_b : public SimdVectorBoolBase< AvxVec4d_b > 
{
public:

    AvxVec4d_b()
    {}

    inline AvxVec4d_b( bool val ) : 
    mValue( _mm256_castsi256_pd( _mm256_set1_epi64x( -(S64)val ) ) )
    {
    }
    
    /*
    inline AvxVec4d_b( bool b0, bool b1, bool b2, bool b3) :
    mValue( _mm256_castsi256_pd( _mm256_setr_epi64x( -(S64)b0, -(S64)b1, -(S64)b2, -(S64)b3 ) ) )
    {
    }
    */
    
    inline AvxVec4d_b( const __m256d &rhs ) :
    mValue( rhs )
    {
    }

    inline AvxVec4d_b &operator=( const __m256d &rhs )
    {
        mValue = rhs;
        
        return *this;
    }
    
    inline operator __m256d () const
    {
        return mValue;
    }
    
    //template< U32 rotate >
    inline void LoadMask( U32 rotate, U64 mask  ) 
    {
        U32 shift = rotate >> 1;
           
        const U32 offset1 = ( shift << 1 );
        const U32 offset2 = ( ( shift ^ 1 ) << 1 );
            
        const U32 rot0 = ( ( 0 + rotate ) & 0x01 );
        const U32 rot1 = ( ( 1 + rotate ) & 0x01 );

        mValue = _mm256_castsi256_pd( _mm256_setr_epi64x( 
                -(S64)( ( mask >> ( 0 +  rot0 + offset1 ) ) & 0x1 ),  
                -(S64)( ( mask >> ( 4 +  rot1 + offset1 ) ) & 0x1 ),   
                -(S64)( ( mask >> ( 8 +  rot0 + offset2 ) ) & 0x1 ), 
                -(S64)( ( mask >> ( 12 + rot1 + offset2 ) ) & 0x1 )
                ) );      
    }
    
    // TODO: MIGHT CAUSE PROBLEMS WITH MANUAL LOADED VALS
    inline U64 StoreMask() const
    {       
        return (U64)_mm256_movemask_pd( mValue );
    }
     
    void StoreAligned( F64 *dest ) const
    {
        _mm256_store_pd( dest, mValue );
    }

    void LoadAligned( const F64 *src ) 
    {
        mValue = _mm256_load_pd( src );
    }

private:

    __m256d mValue;
};

inline AvxVec4d_b operator&( const AvxVec4d_b &lhs, const AvxVec4d_b &rhs )
{
    return _mm256_and_pd( lhs, rhs );
}

inline AvxVec4d_b operator|( const AvxVec4d_b &lhs, const AvxVec4d_b &rhs )
{
    return _mm256_or_pd( lhs, rhs );
}

inline AvxVec4d_b operator^( const AvxVec4d_b &lhs, const AvxVec4d_b &rhs )
{
    return _mm256_xor_pd( lhs, rhs );
}

inline AvxVec4d_b operator==( const AvxVec4d_b &lhs, const AvxVec4d_b &rhs )
{
    return _mm256_cmp_pd( lhs, rhs, 16 );
}

inline AvxVec4d_b operator!=( const AvxVec4d_b &lhs, const AvxVec4d_b &rhs )
{
    return _mm256_cmp_pd( lhs, rhs, 28 );
}

inline bool SIMD_Hadd( const AvxVec4d_b &lhs )
{
    return (bool)_mm256_movemask_pd( lhs );
}

#endif