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
#ifndef __VEC3_H__
#define __VEC3_H__

#include "math/config.h"
#include "math/scalar/vec2.h"
#include "math/generic/floatingPointVector.h"

BEGIN_MATH_NAMESPACE

class Vec3 : public FloatingPointVector< Real, 3 >
{
public:

    FORCE_INLINE Vec3()
    {
        
    }
    
    FORCE_INLINE Vec3( Real p1 )
    {
        SetValues( p1, p1, p1 );
    }
    
    FORCE_INLINE Vec3( Real p1, Real p2, Real p3 )
    {
        SetValues( p1, p2, p3 );
    }
    
    FORCE_INLINE Vec3( const Vec2 &v1, Real p3 )
    {
        SetValues( v1[0], v1[1], p3 );
    }
    
    FORCE_INLINE Vec3( Real p1, const Vec2 &v1 )
    {
        SetValues( p1, v1[0], v1[1] );
    }
    
    template< class TYPE2 >
    FORCE_INLINE Vec3( const PrimaryVectorBase< TYPE2, 3 > &v1 )
    {
        SetValues( (Real) v1[0], (Real) v1[1], (Real) v1[2] );
    }

    FORCE_INLINE Vec3(const FloatingPointVector< Real, 3 > &v1)
    {
        SetValues(v1[0], v1[1], v1[2]);
    }

    // Vec3 specific intrinsics
    FORCE_INLINE Vec3 Rotate(const Vec3 &axis, const Real angle) const
    {
        Vec3 v1 = axis * axis.Dot(*this);
        return v1 + (*this - v1) * Mathf::Cos(angle) + axis.Cross(*this) * Mathf::Sin(angle);
    }

    FORCE_INLINE Vec3 Cross(const Vec3 &v) const
    {
        return Vec3( mValues[1] * v.mValues[2] - mValues[2] * v.mValues[1],
                     mValues[2] * v.mValues[0] - mValues[0] * v.mValues[2],
                     mValues[0] * v.mValues[1] - mValues[1] * v.mValues[0]);
    }
    
    static FORCE_INLINE Vec3 GetZero()
    {
        return Vec3( 0.0f, 0.0f, 0.0f );
    }
    
    static FORCE_INLINE Vec3 GetOne()
    {
        return Vec3( 0.0f, 0.0f, 0.0f );
    }
    
    static FORCE_INLINE Vec3 GetBack()
    {
        return Vec3( 0.0f, -1.0f, 0.0f );
    }
    
    static FORCE_INLINE Vec3 GetForward()
    {
        return Vec3( 0.0f, 1.0f, 0.0f );
    }
    
    static FORCE_INLINE Vec3 GetDown()
    {
        return Vec3( 0.0f, 0.0f, -1.0f );
    }
    
    static FORCE_INLINE Vec3 GetUp()
    {
        return Vec3( 0.0f, 0.0f, 1.0f );
    }
    
    static FORCE_INLINE Vec3 GetLeft()
    {
        return Vec3( -1.0f, 0.0f, 0.0f );
    }
    
    static FORCE_INLINE Vec3 GetRight()
    {
        return Vec3( 1.0f, 0.0f, 0.0f );
    }
};

/*

#include "math/types.h"
#include "math/scalar/mathf.h"
#include "math/scalar/vec3i.h"
#include "math/scalar/vec3.h"

#include <assert.h>

class Vec3
{
    friend Vec3 operator+( const Vec3 &, const Vec3 & );
    friend Vec3 operator*( const Vec3 &, const Vec3 & );
    friend Vec3 operator-( const Vec3 &, const Vec3 & );
    friend Vec3 operator-( const Vec3 & );
    friend Vec3 operator*( const Vec3 &, const Real );
    friend Vec3 operator*( const Real , const Vec3 & );
    friend Vec3 operator/( const Vec3 &, const Real );
    friend Vec3 operator/( const Vec3 &, const Vec3 & );

public:

    FORCE_INLINE Vec3()
    {
    }
    
    FORCE_INLINE Vec3( const Real x, const Real y, const Real z )
    {
        mValues[0] = x;
        mValues[1] = y;
        mValues[2] = z;
    }
    
    FORCE_INLINE Vec3( const Vec3 &v )
    {
        mValues[0] = v.mValues[0];
        mValues[1] = v.mValues[1];
        mValues[2] = v.mValues[2];
    }
    
    FORCE_INLINE Vec3( const Vec3I &v )
    {
        mValues[0] = ( Real )v.GetX();
        mValues[1] = ( Real )v.GetY();
        mValues[2] = ( Real )v.GetZ();
    }
    
    FORCE_INLINE const Real & operator[]( U32 i ) const
    {
        assert( i < 3 );
        
        return mValues[i];
    }
    
    FORCE_INLINE Real &operator[]( U32 i )
    {
        assert( i < 3 );
        
        return mValues[i];
    }
    
    FORCE_INLINE Vec3 &operator=( const Vec3 &other )
    {
        mValues[0] = other.mValues[0];
        mValues[1] = other.mValues[1];
        mValues[2] = other.mValues[2];
    
        return *this;
    }
    
    FORCE_INLINE Vec3 &operator-=( const Vec3 &v )
    {
        mValues[0] -= v.mValues[0];
        mValues[1] -= v.mValues[1];
        mValues[2] -= v.mValues[2];
    
        return *this;
    }
    
    FORCE_INLINE Vec3 &operator+=( const Vec3 &v )
    {
        mValues[0] += v.mValues[0];
        mValues[1] += v.mValues[1];
        mValues[2] += v.mValues[2];
    
        return *this;
    }
    
    FORCE_INLINE Vec3 &operator/=( const Real s )
    {
        assert( s != 0.0f );
    
        mValues[0] /= s;
        mValues[1] /= s;
        mValues[2] /= s;
    
        return *this;
    }
    
    FORCE_INLINE Vec3 &operator*=( const Real s )
    {
        mValues[0] *= s;
        mValues[1] *= s;
        mValues[2] *= s;
    
        return *this;
    }
    
    FORCE_INLINE bool operator==( const Vec3 &other ) const
    {
        return Mathf::Equal( mValues[0], other.mValues[0] ) && Mathf::Equal( mValues[1], other.mValues[1] ) &&
               Mathf::Equal( mValues[2], other.mValues[2] );
    }
    
    FORCE_INLINE bool operator!=( const Vec3 &other ) const
    {
        return !( *this == other );
    }
    
    FORCE_INLINE Real Dot( const Vec3 &v ) const
    {
        return mValues[0] * v.mValues[0] + mValues[1] * v.mValues[1] + mValues[2] * v.mValues[2];
    }
    
    FORCE_INLINE Real Length2() const
    {
        return Dot( *this );
    }
    
    FORCE_INLINE Real Length() const
    {
        return Mathf::Sqrt( Length2() );
    }
    
    FORCE_INLINE Real Distance2( const Vec3 &v ) const
    {
        return ( *this - v ).Length2();
    }
    
    FORCE_INLINE Real Distance( const Vec3 &v ) const
    {
        return ( *this - v ).Length();
    }
    
    FORCE_INLINE void SetValue( const Real x, const Real y, const Real z )
    {
        mValues[0] = x;
        mValues[1] = y;
        mValues[2] = z;
    }
    
    FORCE_INLINE Vec3 SafeNormalise()
    {
        Vec3 absv = Absolute();
        U8 max = absv.MaxAxis();
    
        if ( absv.mValues[max] > 0 )
        {
            assert( absv[max] != 0.0f );
            assert( Length() != 0.0f );
    
            *this /= absv.mValues[max];
            return *this /= Length();
        }
    
        SetValue( 1.0f, 0.0f, 0.0f );
        return *this;
    }
    
    FORCE_INLINE Vec3 Normalise()
    {
        assert( Length() != 0.0f );
    
        return *this /= Length();
    }
    
    FORCE_INLINE Vec3 Lerp( const Vec3 &v, const Real t ) const
    {
        return Vec3( Mathf::Lerp( mValues[0], v.mValues[0], t ), Mathf::Lerp( mValues[1], v.mValues[1], t ),
                     Mathf::Lerp( mValues[2], v.mValues[2], t ) );
    }
    
    FORCE_INLINE Vec3 Slerp( const Vec3 &v, const Real t ) const
    {
        Real dotp = Dot( v );
    
        // Make sure the floating point accuracy doesn't
        // crash our program
        Mathf::Clamp( dotp , -1.0f, 1.0f );
    
        Real theta = Mathf::Acos( dotp ) * t;
        Vec3 relative = v - *this * dotp;
        relative.Normalise();
    
        return *this * Mathf::Cos( theta ) + relative * Mathf::Sin( theta );
    
    }
    
    FORCE_INLINE Vec3 Nlerp( const Vec3 &v, const Real t ) const
    {
        return Lerp( v, t ).Normalise();
    }
    
    FORCE_INLINE Vec3 Rotate( const Vec3 &axis, const Real angle ) const
    {
        Vec3 v1 = axis * axis.Dot( *this );
        return v1 + ( *this - v1 ) * Mathf::Cos( angle ) + axis.Cross( *this ) * Mathf::Sin( angle );
    }
    
    FORCE_INLINE Vec3 Cross( const Vec3 &v ) const
    {
        return Vec3( mValues[1] * v.mValues[2] - mValues[2] * v.mValues[1],
                     mValues[2] * v.mValues[0] - mValues[0] * v.mValues[2],
                     mValues[0] * v.mValues[1] - mValues[1] * v.mValues[0] );
    }
    
    FORCE_INLINE Vec3 Absolute() const
    {
        return Vec3( Mathf::Abs( mValues[0] ), Mathf::Abs( mValues[1] ), Mathf::Abs( mValues[2] ) );
    }
    
    FORCE_INLINE U8 MinAxis() const
    {
        return mValues[0] < mValues[1] ? ( mValues[0] < mValues[2] ? 0 : 2 ) : ( mValues[1] < mValues[2] ? 1 : 2 );
    }
    
    FORCE_INLINE U8 MaxAxis() const
    {
        return mValues[0] < mValues[1] ? ( mValues[1] < mValues[2] ? 2 : 1 ) : ( mValues[0] < mValues[2] ? 2 : 0 );
    }
    
    FORCE_INLINE Real Angle( const Vec3 &v ) const
    {
        Real s = Mathf::Sqrt( Length2() * v.Length2() );
    
        assert( s != 0.0f );
    
        return Mathf::Acos( Mathf::Clamp( Dot( v ) / s, -1.0f, 1.0f ) );
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
    
    FORCE_INLINE void SetZero()
    {
        SetValue( 0.0f, 0.0f, 0.0f );
    }
    
    FORCE_INLINE bool IsZero() const
    {
        return mValues[0] == 0.0f && mValues[1] == 0.0f && mValues[2] == 0.0f;
    }
    
    FORCE_INLINE bool IsFuzzyZero() const
    {
        return Length2() < Mathf::GetEpsilon();
    }
    
    FORCE_INLINE void Clear()
    {
        SetValue( 0.0f, 0.0f, 0.0f );
    }

    static FORCE_INLINE Vec3 GetZero()
    {
        return Vec3( 0.0f, 0.0f, 0.0f );
    }
    
    static FORCE_INLINE Vec3 GetOne()
    {
        return Vec3( 0.0f, 0.0f, 0.0f );
    }
    
    static FORCE_INLINE Vec3 GetBack()
    {
        return Vec3( 0.0f, -1.0f, 0.0f );
    }
    
    static FORCE_INLINE Vec3 GetForward()
    {
        return Vec3( 0.0f, 1.0f, 0.0f );
    }
    
    static FORCE_INLINE Vec3 GetDown()
    {
        return Vec3( 0.0f, 0.0f, -1.0f );
    }
    
    static FORCE_INLINE Vec3 GetUp()
    {
        return Vec3( 0.0f, 0.0f, 1.0f );
    }
    
    static FORCE_INLINE Vec3 GetLeft()
    {
        return Vec3( -1.0f, 0.0f, 0.0f );
    }
    
    static FORCE_INLINE Vec3 GetRight()
    {
        return Vec3( 1.0f, 0.0f, 0.0f );
    }

private:

    Real mValues[3];
};

FORCE_INLINE Vec3 operator+( const Vec3 &v1, const Vec3 &v2 )
{
    return Vec3( v1.mValues[0] + v2.mValues[0],
                 v1.mValues[1] + v2.mValues[1],
                 v1.mValues[2] + v2.mValues[2] );
}

FORCE_INLINE Vec3 operator*( const Vec3 &v1, const Vec3 &v2 )
{
    return Vec3( v1.mValues[0] * v2.mValues[0],
                 v1.mValues[1] * v2.mValues[1],
                 v1.mValues[2] * v2.mValues[2] );
}

FORCE_INLINE Vec3 operator-( const Vec3 &v1, const Vec3 &v2 )
{
    return Vec3( v1.mValues[0] - v2.mValues[0],
                 v1.mValues[1] - v2.mValues[1],
                 v1.mValues[2] - v2.mValues[2] );
}

FORCE_INLINE Vec3 operator-( const Vec3 &v )
{
    return Vec3( -v.mValues[0], -v.mValues[1], -v.mValues[2] );
}

FORCE_INLINE Vec3 operator*( const Vec3 &v, const Real s )
{
    return Vec3( v.mValues[0] * s, v.mValues[1] * s, v.mValues[2] * s );
}

FORCE_INLINE Vec3 operator*( Real s, const Vec3 &v )
{
    return v * s;
}

FORCE_INLINE Vec3 operator/( const Vec3 &v, Real s )
{
    assert( s != 0.0f );
    return v * ( Real( 1.0 ) / s );
}

FORCE_INLINE Vec3 operator/( const Vec3 &v1, const Vec3 &v2 )
{
    return Vec3( v1.mValues[0] / v2.mValues[0],
                 v1.mValues[1] / v2.mValues[1] ,
                 v1.mValues[2] / v2.mValues[2] );
}
*/

#endif