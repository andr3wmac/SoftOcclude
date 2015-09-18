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
#ifndef __VEC3I_H__
#define __VEC3I_H__

#include "math/config.h"
#include "math/scalar/vec2i.h"
#include "math/generic/primaryVectorBase.h"

BEGIN_MATH_NAMESPACE

class Vec3i : public PrimaryVectorBase< S32, 3 >
{
public:

    FORCE_INLINE Vec3i()
    {
        
    }
    
    FORCE_INLINE Vec3i( S32 p1 )
    {
        SetValues( p1, p1, p1 );
    }
    
    FORCE_INLINE Vec3i( S32 p1, S32 p2, S32 p3 )
    {
        SetValues( p1, p2, p3 );
    }
    
    FORCE_INLINE Vec3i( const Vec2i &v1, S32 p3 )
    {
        SetValues( v1[0], v1[1], p3 );
    }
    
    FORCE_INLINE Vec3i( S32 p1, const Vec2i &v1 )
    {
        SetValues( p1, v1[0], v1[1] );
    }
     
    template< class TYPE2 >
    FORCE_INLINE Vec3i( const PrimaryVectorBase< TYPE2, 3 > &v1 )
    {
        SetValues( (S32) v1[0], (S32) v1[1], (S32) v1[2] );
    }

    FORCE_INLINE Vec3i(const PrimaryVectorBase< S32, 3 > &v1)
    {
        SetValues(v1[0], v1[1], v1[2]);
    }
        
};

/*
#include "math/types.h"

#include <assert.h>

class Vec3I
{
    friend Vec3I operator+( const Vec3I &, const Vec3I & );
    friend Vec3I operator*( const Vec3I &, const Vec3I & );
    friend Vec3I operator-( const Vec3I &, const Vec3I & );
    friend Vec3I operator-( const Vec3I & );
    friend Vec3I operator*( const Vec3I &, const Real );
    friend Vec3I operator*( const Real , const Vec3I & );
    friend Vec3I operator/( const Vec3I &, const Real );
    friend Vec3I operator/( const Vec3I &, const Vec3I & );

public:

    FORCE_INLINE Vec3I()
    {
        mValues[0] = 0;
        mValues[1] = 0;
        mValues[2] = 0;
    }
    
    FORCE_INLINE Vec3I( const S32 x, const S32 y, const S32 z )
    {
        SetValue( x, y, z );
    }
    
    FORCE_INLINE Vec3I( Real x, Real y, Real z )
    {
        SetValue( ( S32 )x, ( S32 )y, ( S32 )z );
    }
    
    FORCE_INLINE S32 &operator[]( const U8 axis )
    {
        return mValues[ axis ];
    }
    
    FORCE_INLINE const S32 &operator[]( const U8 axis ) const
    {
        return mValues[ axis ];
    }
    
    FORCE_INLINE void SetX( const S32 val )
    {
        mValues[0] = val;
    }
    
    FORCE_INLINE S32 GetX() const
    {
        return mValues[0];
    }
    
    FORCE_INLINE void SetY( const S32 val )
    {
        mValues[1] = val;
    }
    
    FORCE_INLINE S32 GetY() const
    {
        return mValues[1];
    }
    
    FORCE_INLINE void SetZ( const S32 val )
    {
        mValues[2] = val;
    }
    
    FORCE_INLINE S32 GetZ() const
    {
        return mValues[2];
    }
    
    FORCE_INLINE void SetValue( S32 x, S32 y, S32 z )
    {
        mValues[0] = x;
        mValues[1] = y;
        mValues[2] = z;
    }

    FORCE_INLINE void Clear()
    {
        SetValue( 0, 0, 0 );
    }

    static FORCE_INLINE Vec3I GetZero()
    {
        return Vec3I( 0, 0, 0 );
    }
    
    static FORCE_INLINE Vec3I GetOne()
    {
        return Vec3I( 0, 0, 0 );
    }
    
    static FORCE_INLINE Vec3I GetBack()
    {
        return Vec3I( 0, -1, 0 );
    }
    
    static FORCE_INLINE Vec3I GetForward()
    {
        return Vec3I( 0, 1, 0 );
    }
    
    static FORCE_INLINE Vec3I GetDown()
    {
        return Vec3I( 0, 0, -1 );
    }
    
    static FORCE_INLINE Vec3I GetUp()
    {
        return Vec3I( 0, 0, 1 );
    }
    
    static FORCE_INLINE Vec3I GetLeft()
    {
        return Vec3I( -1, 0, 0 );
    }
    
    static FORCE_INLINE Vec3I GetRight()
    {
        return Vec3I( 1, 0, 0 );
    }

private:

    S32 mValues[3];
};

FORCE_INLINE Vec3I operator+( const Vec3I &v1, const Vec3I &v2 )
{
    return Vec3I( v1.mValues[0] + v2.mValues[0],
                  v1.mValues[1] + v2.mValues[1],
                  v1.mValues[2] + v2.mValues[2] );
}

FORCE_INLINE Vec3I operator*( const Vec3I &v1, const Vec3I &v2 )
{
    return Vec3I( v1.mValues[0] * v2.mValues[0],
                  v1.mValues[1] * v2.mValues[1],
                  v1.mValues[2] * v2.mValues[2] );
}

FORCE_INLINE Vec3I operator-( const Vec3I &v1, const Vec3I &v2 )
{
    return Vec3I( v1.mValues[0] - v2.mValues[0],
                  v1.mValues[1] - v2.mValues[1],
                  v1.mValues[2] - v2.mValues[2] );
}

FORCE_INLINE Vec3I operator-( const Vec3I &v )
{
    return Vec3I( -v.mValues[0], -v.mValues[1], -v.mValues[2] );
}

FORCE_INLINE Vec3I operator*( const Vec3I &v, const Real s )
{
    return Vec3I( v.mValues[0] * s, v.mValues[1] * s, v.mValues[2] * s );
}

FORCE_INLINE Vec3I operator*( Real s, const Vec3I &v )
{
    return v * s;
}

FORCE_INLINE Vec3I operator/( const Vec3I &v, Real s )
{
    assert( s != 0.0f );
    return v * ( Real( 1.0 ) / s );
}

FORCE_INLINE Vec3I operator/( const Vec3I &v1, const Vec3I &v2 )
{
    return Vec3I( v1.mValues[0] / v2.mValues[0],
                  v1.mValues[1] / v2.mValues[1] ,
                  v1.mValues[2] / v2.mValues[2] );
}
*/

#endif