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
#ifndef __VEC4I_H__
#define __VEC4I_H__

#include "math/config.h"
#include "math/scalar/vec2i.h"
#include "math/scalar/vec3i.h"
#include "math/generic/primaryVectorBase.h"

BEGIN_MATH_NAMESPACE

class Vec4i : public PrimaryVectorBase< S32, 4 >
{
public:

    Vec4i()
    {
        
    }
    
    Vec4i( S32 p1 )
    {
        SetValues( p1, p1, p1, p1 );
    }
    
    Vec4i( S32 p1, S32 p2, S32 p3, S32 p4 )
    {
        SetValues( p1, p2, p3, p4 );
    }
    
    Vec4i( const Vec3i &v1, S32 p4 )
    {
        SetValues( v1[0], v1[1], v1[2], p4 );
    }
    
    Vec4i( S32 p1, const Vec3i &v1 )
    {
        SetValues( p1, v1[0], v1[1], v1[2] );
    }
    
    Vec4i( const Vec2i &v1, const Vec2i &v2 )
    {
        SetValues( v1[0], v1[1], v2[0], v2[1] );
    }
    
    template< class TYPE2 >
    inline Vec4i( const PrimaryVectorBase< TYPE2, 4 > &v1 )
    {
        SetValues( (S32) v1[0], (S32) v1[1], (S32) v1[2], (S32) v1[3] );
    }

    Vec4i(const PrimaryVectorBase< S32, 4 > &v1)
    {
        SetValues(v1[0], v1[1], v1[2], v1[3]);
    }
        
};

/*
#include <cstddef>

#include "math/types.h"

#include <assert.h>

class Vec4I
{
    friend Vec4I operator+( const Vec4I &, const Vec4I & );
    friend Vec4I operator*( const Vec4I &, const Vec4I & );
    friend Vec4I operator-( const Vec4I &, const Vec4I & );
    friend Vec4I operator-( const Vec4I & );
    friend Vec4I operator*( const Vec4I &, const Real );
    friend Vec4I operator*( const Real , const Vec4I & );
    friend Vec4I operator/( const Vec4I &, const Real );
    friend Vec4I operator/( const Vec4I &, const Vec4I & );

public:

    inline Vec4I()
    {
        mValues[0] = 0;
        mValues[1] = 0;
        mValues[2] = 0;
        mValues[3] = 0;
    }
    
    
    inline Vec4I( const S32 &x, const S32 &y, const S32 &z, const S32 &w )
    {
        SetValue( x, y, z, w );
    }
    
    inline Vec4I( Real x, Real y, Real z, Real w )
    {
        SetValue( ( S32 )x, ( S32 )y, ( S32 )z, ( S32 )w );
    }
    
    inline S32 &operator[]( const size_t axis )
    {
        return mValues[ axis ];
    }
    
    inline const S32 &operator[]( const size_t axis ) const
    {
        return mValues[ axis ];
    }
    
    inline S32 GetX() const
    {
        return mValues[0];
    }
    
    inline void SetX( const S32 &val )
    {
        mValues[0] = val;
    }
    
    inline S32 GetY() const
    {
        return mValues[1];
    }
    
    inline void SetY( const S32 &val )
    {
        mValues[1] = val;
    }
    
    inline S32 GetZ() const
    {
        return mValues[2];
    }
    
    inline void SetZ( const S32 &val )
    {
        mValues[2] = val;
    }
    
    inline S32 GetW() const
    {
        return mValues[3];
    }
    
    inline void SetW( const S32 &val )
    {
        mValues[3] = val;
    }
    
    inline void SetValue( const S32 &x, const S32 &y, const S32 &z, const S32 &w )
    {
        mValues[0] = x;
        mValues[1] = y;
        mValues[2] = z;
        mValues[3] = w;
    }
    
    inline void Clear()
    {
        SetValue( 0, 0, 0, 0 );
    }
    
    static inline Vec4I GetZero()
    {
        return Vec4I( 0, 0, 0, 0 );
    }
    
    static inline Vec4I GetOne()
    {
        return Vec4I( 1, 1, 1, 1 );
    }

private:

    S32 mValues[4];
};

inline Vec4I operator+( const Vec4I &v1, const Vec4I &v2 )
{
    return Vec4I( v1.mValues[0] + v2.mValues[0],
                  v1.mValues[1] + v2.mValues[1],
                  v1.mValues[2] + v2.mValues[2],
                  v1.mValues[3] + v2.mValues[3] );
}

inline Vec4I operator*( const Vec4I &v1, const Vec4I &v2 )
{
    return Vec4I( v1.mValues[0] * v2.mValues[0],
                  v1.mValues[1] * v2.mValues[1],
                  v1.mValues[2] * v2.mValues[2],
                  v1.mValues[3] * v2.mValues[3] );
}

inline Vec4I operator-( const Vec4I &v1, const Vec4I &v2 )
{
    return Vec4I( v1.mValues[0] - v2.mValues[0],
                  v1.mValues[1] - v2.mValues[1],
                  v1.mValues[2] - v2.mValues[2],
                  v1.mValues[3] - v2.mValues[3] );
}

inline Vec4I operator-( const Vec4I &v )
{
    return Vec4I( -v.mValues[0], -v.mValues[1], -v.mValues[2], -v.mValues[3] );
}

inline Vec4I operator*( const Vec4I &v, const Real s )
{
    return Vec4I( v.mValues[0] * s, v.mValues[1] * s, v.mValues[2] * s, v.mValues[3] * s );
}

inline Vec4I operator*( const Real s, const Vec4I &v )
{
    return v * s;
}

inline Vec4I operator/( const Vec4I &v, const Real s )
{
    assert( s != 0.0f );

    return v * ( Real( 1.0 ) / s );
}

inline Vec4I operator/( const Vec4I &v1, const Vec4I &v2 )
{
    return Vec4I( v1.mValues[0] / v2.mValues[0],
                  v1.mValues[1] / v2.mValues[1],
                  v1.mValues[2] / v2.mValues[2],
                  v1.mValues[3] / v2.mValues[3] );
}
*/

#endif