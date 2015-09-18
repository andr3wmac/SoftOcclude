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
#ifndef __AVX2_VEC8B_H__
#define __AVX2_VEC8B_H__

#include "math/types.h"
#include "math/config.h"

#include "math/scalar/mathf.h"

#include "math/simd/vectorize.h"
#include "math/simd/generic/simdVectorBoolBase.h"

#include <immintrin.h>
#include <iostream>
#include <limits>

class AvxVec8f_b : public SimdVectorBoolBase< AvxVec8f_b > 
{
public:

    AvxVec8f_b()
    {}

    FORCE_INLINE AvxVec8f_b( bool val ) : 
    mValue( _mm256_castsi256_ps( _mm256_set1_epi32( -(int)val ) ) )
    {
    }
    
    /*
    FORCE_INLINE AvxVec8f_b( bool b0, bool b1, bool b2, bool b3,
                        bool b4, bool b5, bool b6, bool b7 ) :
    mValue( _mm256_castsi256_ps( _mm256_setr_epi32( -(int)b0, -(int)b1, -(int)b2, -(int)b3,
                                                    -(int)b4, -(int)b5, -(int)b6, -(int)b7 ) ) )
    {
    }
    */
    
    FORCE_INLINE AvxVec8f_b( const __m256 &rhs ) :
    mValue( rhs )
    {
    }

    FORCE_INLINE AvxVec8f_b(const AvxVec8f_b &rhs) :
        mValue(rhs.mValue)
    {
    }

    FORCE_INLINE AvxVec8f_b &operator=( const __m256 &rhs )
    {
        mValue = rhs;
        
        return *this;
    }
    
    FORCE_INLINE operator __m256 () const
    {
        return mValue;
    }
    
    FORCE_INLINE void LoadMask( U32 rotate, U64 mask ) 
    {
        U32 shift = rotate >> 2;
           
        const U32 offset1 = ( shift << 2 );
        const U32 offset2 = ( ( shift ^ 1 ) << 2 );
            
        const U32 rot0 = ( ( 0 + rotate ) & 0x03 );
        const U32 rot1 = ( ( 1 + rotate ) & 0x03 );
        const U32 rot2 = ( ( 2 + rotate ) & 0x03 );
        const U32 rot3 = ( ( 3 + rotate ) & 0x03 );

        mValue = _mm256_castsi256_ps( _mm256_setr_epi32( 
                -(S32)( ( mask >> ( 0 +  rot0 + offset1 ) ) & 0x1 ),  
                -(S32)( ( mask >> ( 8 +  rot1 + offset1 ) ) & 0x1 ),  
                -(S32)( ( mask >> ( 16 + rot2 + offset1 ) ) & 0x1 ),  
                -(S32)( ( mask >> ( 24 + rot3 + offset1 ) ) & 0x1 ), 
                -(S32)( ( mask >> ( 32 + rot0 + offset2 ) ) & 0x1 ), 
                -(S32)( ( mask >> ( 40 + rot1 + offset2 ) ) & 0x1 ), 
                -(S32)( ( mask >> ( 48 + rot2 + offset2 ) ) & 0x1 ), 
                -(S32)( ( mask >> ( 56 + rot3 + offset2 ) ) & 0x1 ) 
                ) );  
        
    }
   
    template< U32 rotate >
    FORCE_INLINE void LoadMask( U64 mask ) 
    {
        U32 shift = rotate >> 2;
           
        const U32 offset1 = ( shift << 2 );
        const U32 offset2 = ( ( shift ^ 1 ) << 2 );
            
        const U32 rot0 = ( ( 0 + rotate ) & 0x03 );
        const U32 rot1 = ( ( 1 + rotate ) & 0x03 );
        const U32 rot2 = ( ( 2 + rotate ) & 0x03 );
        const U32 rot3 = ( ( 3 + rotate ) & 0x03 );

        mValue = _mm256_castsi256_ps( _mm256_setr_epi32( 
                -(S32)( ( mask >> ( 0 +  rot0 + offset1 ) ) & 0x1 ),  
                -(S32)( ( mask >> ( 8 +  rot1 + offset1 ) ) & 0x1 ),  
                -(S32)( ( mask >> ( 16 + rot2 + offset1 ) ) & 0x1 ),  
                -(S32)( ( mask >> ( 24 + rot3 + offset1 ) ) & 0x1 ), 
                -(S32)( ( mask >> ( 32 + rot0 + offset2 ) ) & 0x1 ), 
                -(S32)( ( mask >> ( 40 + rot1 + offset2 ) ) & 0x1 ), 
                -(S32)( ( mask >> ( 48 + rot2 + offset2 ) ) & 0x1 ), 
                -(S32)( ( mask >> ( 56 + rot3 + offset2 ) ) & 0x1 ) 
                ) );      
    }
    
    // TODO: MIGHT CAUSE PROBLEMS WITH MANUAL LOADED VALS
    FORCE_INLINE U64 StoreMask() const
    {       
        return (U64)_mm256_movemask_ps( mValue );
    }
     
    void StoreAligned( F32 *dest ) const
    {
        _mm256_store_ps( dest, mValue );
    }

    void LoadAligned( const F32 *src ) 
    {
        mValue = _mm256_load_ps( src );
    }

private:

    __m256 mValue;
};

FORCE_INLINE AvxVec8f_b operator&( const AvxVec8f_b &lhs, const AvxVec8f_b &rhs )
{
    return _mm256_and_ps( lhs, rhs );
}

FORCE_INLINE AvxVec8f_b operator|( const AvxVec8f_b &lhs, const AvxVec8f_b &rhs )
{
    return _mm256_or_ps( lhs, rhs );
}

FORCE_INLINE AvxVec8f_b operator^( const AvxVec8f_b &lhs, const AvxVec8f_b &rhs )
{
    return _mm256_xor_ps( lhs, rhs );
}

FORCE_INLINE AvxVec8f_b operator==( const AvxVec8f_b &lhs, const AvxVec8f_b &rhs )
{
    return _mm256_cmp_ps( lhs, rhs, 16 );
}

FORCE_INLINE AvxVec8f_b operator!=( const AvxVec8f_b &lhs, const AvxVec8f_b &rhs )
{
    return _mm256_cmp_ps( lhs, rhs, 28 );
}

FORCE_INLINE bool SIMD_Hadd( const AvxVec8f_b &lhs )
{
    return (bool)_mm256_movemask_ps( lhs );
}

#endif