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
#ifndef __VEC4_H__
#define __VEC4_H__

#include "math/config.h"
#include "math/scalar/vec2.h"
#include "math/scalar/vec3.h"
#include "math/generic/floatingPointVector.h"

BEGIN_MATH_NAMESPACE

class Vec4 : public FloatingPointVector< Real, 4 >
{
public:

    Vec4()
    {
        
    }
    
    Vec4( Real p1 )
    {
        SetValues( p1, p1, p1, p1 );
    }
    
    Vec4( Real p1, Real p2, Real p3, Real p4 )
    {
        SetValues( p1, p2, p3, p4 );
    }
    
    Vec4( const Vec2 &v1, const Vec2 &v2 )
    {
        SetValues( v1[0], v1[1], v2[0], v2[1] );
    }
    
    Vec4( Real p1, const Vec3 &v1 )
    {
        SetValues( p1, v1[0], v1[1], v1[2] );
    }
    
    Vec4( const Vec3 &v1, Real p1 )
    {
        SetValues( v1[0], v1[1], v1[2], p1 );
    }
    
    template< class TYPE2 >
    FORCE_INLINE Vec4( const PrimaryVectorBase< TYPE2, 4 > &v1 )
    {
        SetValues( (Real) v1[0], (Real) v1[1], (Real) v1[2], (Real) v1[3] );
    }
    
    Vec4(const FloatingPointVector< Real, 4 > &v1)
    {
        SetValues(v1[0], v1[1], v1[2], v1[3]);
    }
        
};


/*
#include "math/types.h"
#include "math/scalar/mathf.h"
#include "math/scalar/vec4i.h"
#include "math/scalar/vec4.h"
#include "math/scalar/vec3.h"
#include "math/scalar/vec2.h"

#include <assert.h>

class Vec4
{
    friend Vec4 operator+( const Vec4 &, const Vec4 & );
    friend Vec4 operator*( const Vec4 &, const Vec4 & );
    friend Vec4 operator-( const Vec4 &, const Vec4 & );
    friend Vec4 operator-( const Vec4 & );
    friend Vec4 operator*( const Vec4 &, const Real );
    friend Vec4 operator*( const Real , const Vec4 & );
    friend Vec4 operator/( const Vec4 &, const Real );
    friend Vec4 operator/( const Vec4 &, const Vec4 & );

public:

    FORCE_INLINE Vec4()
    {
    }
    
    FORCE_INLINE Vec4( const Real x, const Real y, const Real z, const Real w )
    {
        mValues[0] = x;
        mValues[1] = y;
        mValues[2] = z;
        mValues[3] = w;
    }
    
    FORCE_INLINE Vec4( const Vec4 &v )
    {
        mValues[0] = v.mValues[0];
        mValues[1] = v.mValues[1];
        mValues[2] = v.mValues[2];
        mValues[3] = v.mValues[3];
    }
    
    FORCE_INLINE Vec4( const Vec4I &v )
    {
        mValues[0] = ( Real )v.GetX();
        mValues[1] = ( Real )v.GetY();
        mValues[2] = ( Real )v.GetZ();
        mValues[3] = ( Real )v.GetW();
    }
    
    FORCE_INLINE Vec4( const Vec3 &other, Real w )
    {
        mValues[0] = other[0];
        mValues[1] = other[1];
        mValues[2] = other[2];
        mValues[3] = w;
    }
    
    FORCE_INLINE Vec4( const Vec2 &other1, const Vec2 &other2 )
    {
        mValues[0] = other1[0];
        mValues[1] = other1[1];
    
        mValues[2] = other2[0];
        mValues[3] = other2[1];
    }
    
    FORCE_INLINE Real operator[]( U32 i ) const
    {
        assert( i < 4 );
        
        return mValues[i];
    }
    
    FORCE_INLINE Real &operator[]( U32 i )
    {
        assert( i < 4 );
        
        return mValues[i];
    }
    
    FORCE_INLINE Vec4 &operator=( const Vec4 &other )
    {
        mValues[0] = other.mValues[0];
        mValues[1] = other.mValues[1];
        mValues[2] = other.mValues[2];
        mValues[3] = other.mValues[3];
    
        return *this;
    }
    
    FORCE_INLINE Vec4 &operator-=( const Vec4 &v )
    {
        mValues[0] -= v.mValues[0];
        mValues[1] -= v.mValues[1];
        mValues[2] -= v.mValues[2];
        mValues[3] -= v.mValues[3];
    
        return *this;
    }
    
    FORCE_INLINE Vec4 &operator+=( const Vec4 &v )
    {
        mValues[0] += v.mValues[0];
        mValues[1] += v.mValues[1];
        mValues[2] += v.mValues[2];
        mValues[3] += v.mValues[3];
    
        return *this;
    }
    
    FORCE_INLINE Vec4 &operator/=( Real s )
    {
        assert( s != 0.0f );
    
        mValues[0] /= s;
        mValues[1] /= s;
        mValues[2] /= s;
        mValues[3] /= s;
    
        return *this;
    }
    
    FORCE_INLINE Vec4 &operator*=( Real s )
    {
        mValues[0] *= s;
        mValues[1] *= s;
        mValues[2] *= s;
        mValues[3] *= s;
    
        return *this;
    }
    
    FORCE_INLINE bool operator==( const Vec4 &other ) const
    {
        return Mathf::Equal( mValues[0], other.mValues[0] ) && Mathf::Equal( mValues[1], other.mValues[1] ) &&
               Mathf::Equal( mValues[2], other.mValues[2] ) && Mathf::Equal( mValues[3], other.mValues[3] );
    }
    
    FORCE_INLINE bool operator!=( const Vec4 &other ) const
    {
        return !( *this == other );
    }
    
    FORCE_INLINE Real &operator[]( const U8 axis )
    {
        return mValues[ axis ];
    }
    
    FORCE_INLINE const Real &operator[]( const U8 axis ) const
    {
        return mValues[ axis ];
    }
    
    FORCE_INLINE Real Dot( const Vec4 &v ) const
    {
        return mValues[0] * v.mValues[0] + mValues[1] * v.mValues[1] + mValues[2] * v.mValues[2] + mValues[3] * v.mValues[3];
    }
    
    FORCE_INLINE Real Length2() const
    {
        return Dot( *this );
    }
    
    FORCE_INLINE Real Length() const
    {
        return Mathf::Sqrt( Length2() );
    }
    
    FORCE_INLINE Real Distance2( const Vec4 &v ) const
    {
        return ( *this - v ).Length2();
    }
    
    FORCE_INLINE Real Distance( const Vec4 &v ) const
    {
        return ( *this - v ).Length();
    }
    
    FORCE_INLINE void SetValue( const Real x, const Real y, const Real z, const Real w )
    {
        mValues[0] = x;
        mValues[1] = y;
        mValues[2] = z;
        mValues[3] = w;
    }
    
    FORCE_INLINE Vec4 SafeNormalise()
    {
        Vec4 absv = Absolute();
        U8 max = absv.MaxAxis();
    
        if ( absv.mValues[max] > 0 )
        {
            assert( absv[max] != 0.0f );
            assert( Length() != 0.0f );
    
            *this /= absv.mValues[max];
            return *this /= Length();
        }
    
        SetValue( 1.0f, 0.0f, 0.0f, 0.0f );
        return *this;
    }
    
    FORCE_INLINE Vec4 Normalise()
    {
        return *this /= Length();
    }
    
    FORCE_INLINE Vec4 Lerp( const Vec4 &v, const Real t ) const
    {
        return Vec4( mValues[0] + ( v.mValues[0] - mValues[0] ) * t, mValues[1] + ( v.mValues[1] - mValues[1] ) * t,
                     mValues[2] + ( v.mValues[2] - mValues[2] ) * t, mValues[3] + ( v.mValues[3] - mValues[3] ) * t );
    }
    
    FORCE_INLINE Vec4 Absolute() const
    {
        return Vec4( Mathf::Abs( mValues[0] ), Mathf::Abs( mValues[1] ), Mathf::Abs( mValues[2] ), Mathf::Abs( mValues[3] ) );
    }
    
    FORCE_INLINE U8 MinAxis() const
    {
        U8 axis = 0;
        Real minvalue = mValues[0];
    
        for ( U8 i = 1; i < 4; ++i )
        {
            if ( minvalue > mValues[i] )
            {
                minvalue = mValues[i];
                axis = i;
            }
        }
    
        return axis;
    }
    
    FORCE_INLINE U8 MaxAxis() const
    {
        U8 axis = 0;
        Real maxvalue = mValues[0];
    
        for ( U8 i = 1; i < 4; ++i )
        {
            if ( maxvalue < mValues[i] )
            {
                maxvalue = mValues[i];
                axis = i;
            }
        }
    
        return axis;
    }
    
    FORCE_INLINE void SetX( const Real x )
    {
        mValues[0] = x;
    }
    
    FORCE_INLINE Real GetX() const
    {
        return mValues[0];
    }
    
    FORCE_INLINE void SetY( const Real y )
    {
        mValues[1] = y;
    }
    
    FORCE_INLINE Real GetY() const
    {
        return mValues[1];
    }
    
    FORCE_INLINE void SetZ( const Real z )
    {
        mValues[2] = z;
    }
    
    FORCE_INLINE Real GetZ() const
    {
        return mValues[2];
    }
    
    FORCE_INLINE void SetW( const Real w )
    {
        mValues[3] = w;
    }
    
    FORCE_INLINE Real GetW() const
    {
        return mValues[3];
    }
    
    FORCE_INLINE void SetZero()
    {
        SetValue( 0.0f, 0.0f, 0.0f, 0.0f );
    }
    
    FORCE_INLINE bool IsZero() const
    {
        return mValues[0] == 0.0f && mValues[1] == 0.0f && mValues[2] == 0.0f && mValues[3] == 0.0f;
    }
    
    FORCE_INLINE bool IsFuzzyZero() const
    {
        return Length2() < Mathf::GetEpsilon();
    }
    
    FORCE_INLINE void Clear()
    {
        SetValue( 0.0f, 0.0f, 0.0f, 0.0f );
    }
    
    static FORCE_INLINE Vec4 GetZero()
    {
        return Vec4( 0.0f, 0.0f, 0.0f, 0.0f );
    }
    
    static FORCE_INLINE Vec4 GetOne()
    {
        return Vec4( 1.0f, 1.0f, 1.0f, 1.0f );
    }
    
    FORCE_INLINE Vec3 GetXYZ() const
    {
        return Vec3( mValues[0], mValues[1], mValues[2] );
    }
    
    FORCE_INLINE Real *Data()
    {
        return mValues;
    }
    
    FORCE_INLINE const Real *Data() const
    {
        return mValues;
    }

private:

    Real mValues[4];
};

FORCE_INLINE Vec4 operator+( const Vec4 &v1, const Vec4 &v2 )
{
    return Vec4( v1.mValues[0] + v2.mValues[0],
                 v1.mValues[1] + v2.mValues[1],
                 v1.mValues[2] + v2.mValues[2],
                 v1.mValues[3] + v2.mValues[3] );
}

FORCE_INLINE Vec4 operator*( const Vec4 &v1, const Vec4 &v2 )
{
    return Vec4( v1.mValues[0] * v2.mValues[0],
                 v1.mValues[1] * v2.mValues[1],
                 v1.mValues[2] * v2.mValues[2],
                 v1.mValues[3] * v2.mValues[3] );
}

FORCE_INLINE Vec4 operator-( const Vec4 &v1, const Vec4 &v2 )
{
    return Vec4( v1.mValues[0] - v2.mValues[0],
                 v1.mValues[1] - v2.mValues[1],
                 v1.mValues[2] - v2.mValues[2],
                 v1.mValues[3] - v2.mValues[3] );
}

FORCE_INLINE Vec4 operator-( const Vec4 &v )
{
    return Vec4( -v.mValues[0], -v.mValues[1], -v.mValues[2], -v.mValues[3] );
}

FORCE_INLINE Vec4 operator*( const Vec4 &v, const Real s )
{
    return Vec4( v.mValues[0] * s, v.mValues[1] * s, v.mValues[2] * s, v.mValues[3] * s );
}

FORCE_INLINE Vec4 operator*( const Real s, const Vec4 &v )
{
    return v * s;
}

FORCE_INLINE Vec4 operator/( const Vec4 &v, const Real s )
{
    assert( s != 0.0f );

    return v * ( Real( 1.0 ) / s );
}

FORCE_INLINE Vec4 operator/( const Vec4 &v1, const Vec4 &v2 )
{
    return Vec4( v1.mValues[0] / v2.mValues[0],
                 v1.mValues[1] / v2.mValues[1],
                 v1.mValues[2] / v2.mValues[2],
                 v1.mValues[3] / v2.mValues[3] );
}
*/

#endif