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
#ifndef __FP_VECTOR_H__
#define __FP_VECTOR_H__

#include "math/config.h"
#include "math/generic/primaryVectorBase.h"

BEGIN_MATH_NAMESPACE

template< class TYPE, U32 N>
class FloatingPointVector : public PrimaryVectorBase< TYPE, N >
{
public:

    FloatingPointVector()
    {}

    FloatingPointVector(const PrimaryVectorBase< TYPE, N > &lhs)
    {
        for (U32 i = 0; i < N; ++i)
        {
            this->mValues[i] = lhs[i];
        }
    }

    // Internal intrinsics
    FORCE_INLINE TYPE Distance( const FloatingPointVector< TYPE, N > &v ) const
    {
        FloatingPointVector< TYPE, N > intermediate = (*this - v);

        return intermediate.Length();
    }
    
    FORCE_INLINE TYPE Length() const
    {
        return Mathf::Sqrt( this->Length2() );
    }
    
    FORCE_INLINE FloatingPointVector< TYPE, N > SafeNormalise()
    {
        const FloatingPointVector< TYPE, N > absv = this->Absolute();
        U8 max = absv.MaxAxis();
    
        if ( absv.mValues[max] > 0 )
        {
            assert( absv[max] != 0.0f );
            assert( Length() != 0.0f );
    
            FloatingPointVector< TYPE, N >(*this) /= absv.mValues[max];
            return FloatingPointVector< TYPE, N >(*this) /= Length();
        }
    
        this->SetValue( 0, 1.0f );
        
        for ( U32 i=1; i < N; ++i )
        {
            this->SetValue( i, 0.0f );
        }
        
        return FloatingPointVector< TYPE, N >(*this);
    }
    
    FORCE_INLINE FloatingPointVector< TYPE, N > Normalise()
    {
        assert( Length() != 0.0f );
    
        return FloatingPointVector< TYPE, N >(*this) /= Length();
    }
    
    FORCE_INLINE FloatingPointVector< TYPE, N > Lerp( const FloatingPointVector< TYPE, N > &v, const TYPE t ) const
    {
        FloatingPointVector< TYPE, N > newVec;
        
        for ( U32 i=0; i < N; ++i )
        {
            newVec.SetValue( i,  Mathf::Lerp( this->mValues[i], v.mValues[i], t ) ); 
        }
        
        return newVec;
    }
    
    FORCE_INLINE FloatingPointVector< TYPE, N > Slerp( const FloatingPointVector< TYPE, N > &v, const TYPE t ) const
    {
        TYPE dotp = Dot( v );
    
        // Make sure the floating point accuracy doesn't
        // crash our program
        Mathf::Clamp( dotp , -1.0f, 1.0f );
    
        TYPE theta = Mathf::Acos( dotp ) * t;
        FloatingPointVector< TYPE, N > relative = v - FloatingPointVector< TYPE, N >(*this) * dotp;
        relative.Normalise();
    
        return FloatingPointVector< TYPE, N >(*this) * Mathf::Cos( theta ) + relative * Mathf::Sin( theta );
    }
    
    FORCE_INLINE FloatingPointVector< TYPE, N > Nlerp( const FloatingPointVector< TYPE, N > &v, const TYPE t ) const
    {
        return Lerp( v, t ).Normalise();
    }
    
    FORCE_INLINE TYPE Angle( const FloatingPointVector< TYPE, N > &v ) const
    {
        TYPE s = Mathf::Sqrt( this->Length2() * v.Length2() );
    
        assert( s != 0.0f );
    
        return Mathf::Acos( Mathf::Clamp( Dot( v ) / s, -1.0f, 1.0f ) );
    }
};

END_MATH_NAMESPACE

#endif