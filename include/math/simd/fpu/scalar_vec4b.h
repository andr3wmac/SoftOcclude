#pragma once
#ifndef __SCALAR_VEC_4B_H__
#define __SCALAR_VEC_4B_H__

#include "math/types.h"

#include "math/simd/generic/simdVectorBoolBase.h"

#include <algorithm> 

class ScalarVec4b : public SimdVectorBoolBase< ScalarVec4b > 
{
public:

    ScalarVec4b()
    {}

    inline ScalarVec4b( bool val )
    {
        for ( U32 i=0; i < 4; ++i )
        {
            mValue[i] = val;
        }
    }
    
    
    inline ScalarVec4b( const bool *rhs ) 
    {
    	std::copy( rhs,rhs+4, mValue );
    }

    inline ScalarVec4b &operator=( const bool *rhs )
    {
        std::copy( rhs,rhs+4, mValue );

        return *this;
    }
    
    inline operator bool* ()
    {
        return mValue;
    }
    
    inline operator const bool* () const
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
    

    inline void LoadMask( U32 rotate, U64 mask ) 
    {
        mValue[0] = ( mask >> ( 0  + ( 0 + rotate ) % 4 ) ) & 0x1;
        mValue[1] = ( mask >> ( 4  + ( 1 + rotate ) % 4 ) ) & 0x1;
        mValue[2] = ( mask >> ( 8  + ( 2 + rotate ) % 4 ) ) & 0x1;
        mValue[3] = ( mask >> ( 12 + ( 3 + rotate ) % 4 ) ) & 0x1;                    
    }
    
    inline U64 StoreMask() const
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

inline ScalarVec4b operator&( const ScalarVec4b &lhs, const ScalarVec4b &rhs )
{
    ScalarVec4b newVec;
    
    for ( U32 i=0; i < 4; ++i ) 
    {
        newVec()[i] = lhs()[i] & rhs()[i];
    }
    
    return newVec;
}

inline ScalarVec4b operator|( const ScalarVec4b &lhs, const ScalarVec4b &rhs )
{
    ScalarVec4b newVec;
    
    for ( U32 i=0; i < 4; ++i ) 
    {
        newVec()[i] = lhs()[i] | rhs()[i];
    }
    
    return newVec;
}

inline ScalarVec4b operator^( const ScalarVec4b &lhs, const ScalarVec4b &rhs )
{
    ScalarVec4b newVec;
    
    for ( U32 i=0; i < 4; ++i ) 
    {
        newVec()[i] = lhs()[i] ^ rhs()[i];
    }
    
    return newVec;
}

inline ScalarVec4b operator==( const ScalarVec4b &lhs, const ScalarVec4b &rhs )
{
    ScalarVec4b newVec;
    
    for ( U32 i=0; i < 4; ++i ) 
    {
        newVec()[i] = lhs()[i] == rhs()[i];
    }
    
    return newVec;
}

inline ScalarVec4b operator!=( const ScalarVec4b &lhs, const ScalarVec4b &rhs )
{
    ScalarVec4b newVec;
    
    for ( U32 i=0; i < 4; ++i ) 
    {
        newVec()[i] = lhs()[i] != rhs()[i];
    }
    
    return newVec;
}

inline bool SIMD_Hadd( const ScalarVec4b &lhs )
{
    int val = 0;
    
    for ( U32 i=0; i < 4; ++i ) 
    {
        val += lhs()[i];
    }
    
    return (bool)val;
}


#endif