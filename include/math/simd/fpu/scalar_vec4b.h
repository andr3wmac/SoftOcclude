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
#ifndef __SCALAR_VEC_4B_H__
#define __SCALAR_VEC_4B_H__

#include "math/types.h"
#include "math/config.h"

#include "math/simd/generic/simdVectorBoolBase.h"

#include <algorithm> 

class ScalarVec4b : public SimdVectorBoolBase< ScalarVec4b > 
{
public:

    ScalarVec4b()
    {}

    FORCE_INLINE ScalarVec4b( bool val )
    {
        for ( U32 i=0; i < 4; ++i )
        {
            mValue[i] = val;
        }
    }
    
    
    FORCE_INLINE ScalarVec4b( const bool *rhs ) 
    {
    	std::copy( rhs,rhs+4, mValue );
    }

    FORCE_INLINE ScalarVec4b &operator=( const bool *rhs )
    {
        std::copy( rhs,rhs+4, mValue );

        return *this;
    }
    
    FORCE_INLINE operator bool* ()
    {
        return mValue;
    }
    
    FORCE_INLINE operator const bool* () const
    {
        return mValue;
    }
    
    void StoreAligned( Real *dest ) const
    {
        for ( U32 i=0; i < 4; ++i ) 
        {
            dest[i] = (Real) mValue[i];
        }
    }

    void LoadAligned( const Real *src ) 
    {
        for ( U32 i=0; i < 4; ++i ) 
        {
            mValue[i] = (bool) src[i];
        }
    }
    

    FORCE_INLINE void LoadMask( U32 rotate, U64 mask ) 
    {
        mValue[0] = ( mask >> ( 0  + ( 0 + rotate ) % 4 ) ) & 0x1;
        mValue[1] = ( mask >> ( 4  + ( 1 + rotate ) % 4 ) ) & 0x1;
        mValue[2] = ( mask >> ( 8  + ( 2 + rotate ) % 4 ) ) & 0x1;
        mValue[3] = ( mask >> ( 12 + ( 3 + rotate ) % 4 ) ) & 0x1;                    
    }
    
    FORCE_INLINE U64 StoreMask() const
    {   
        U64 mask = 0;    
        
        mask |= ( (U64)mValue[0] & 0x1 ) << 0;
        mask |= ( (U64)mValue[1] & 0x1 ) << 1;
        mask |= ( (U64)mValue[2] & 0x1 ) << 2;
        mask |= ( (U64)mValue[3] & 0x1 ) << 3;
        
        return mask;
    }
    
private:

    bool mValue[4];
};

FORCE_INLINE ScalarVec4b operator&( const ScalarVec4b &lhs, const ScalarVec4b &rhs )
{
    ScalarVec4b newVec;
    
    for ( U32 i=0; i < 4; ++i ) 
    {
        newVec()[i] = lhs()[i] & rhs()[i];
    }
    
    return newVec;
}

FORCE_INLINE ScalarVec4b operator|( const ScalarVec4b &lhs, const ScalarVec4b &rhs )
{
    ScalarVec4b newVec;
    
    for ( U32 i=0; i < 4; ++i ) 
    {
        newVec()[i] = lhs()[i] | rhs()[i];
    }
    
    return newVec;
}

FORCE_INLINE ScalarVec4b operator^( const ScalarVec4b &lhs, const ScalarVec4b &rhs )
{
    ScalarVec4b newVec;
    
    for ( U32 i=0; i < 4; ++i ) 
    {
        newVec()[i] = lhs()[i] ^ rhs()[i];
    }
    
    return newVec;
}

FORCE_INLINE ScalarVec4b operator==( const ScalarVec4b &lhs, const ScalarVec4b &rhs )
{
    ScalarVec4b newVec;
    
    for ( U32 i=0; i < 4; ++i ) 
    {
        newVec()[i] = lhs()[i] == rhs()[i];
    }
    
    return newVec;
}

FORCE_INLINE ScalarVec4b operator!=( const ScalarVec4b &lhs, const ScalarVec4b &rhs )
{
    ScalarVec4b newVec;
    
    for ( U32 i=0; i < 4; ++i ) 
    {
        newVec()[i] = lhs()[i] != rhs()[i];
    }
    
    return newVec;
}

FORCE_INLINE bool SIMD_Hadd( const ScalarVec4b &lhs )
{
    int val = 0;
    
    for ( U32 i=0; i < 4; ++i ) 
    {
        val += lhs()[i];
    }
    
    return (bool)val;
}


#endif