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
#ifndef __VEC2I_H__
#define __VEC2I_H__

#include "math/config.h"
#include "math/generic/primaryVectorBase.h"

BEGIN_MATH_NAMESPACE

class Vec2i : public PrimaryVectorBase< S32, 2 >
{
public:

    inline Vec2i()
    {
        
    }
    
    inline Vec2i( S32 p1 )
    {
        SetValues( p1, p1 );
    }
    
    inline Vec2i( S32 p1, S32 p2 )
    {
        SetValues( p1, p2 );
    }
    
    template< class TYPE2 >
    inline Vec2i( const PrimaryVectorBase< TYPE2, 2 > &v1 )
    {
        SetValues( (S32) v1[0], (S32) v1[1] );
    }

    inline Vec2i(const PrimaryVectorBase< S32, 2 > &v1)
    {
        SetValues(v1[0], v1[1]);
    }
  
};

/*
#include "math/types.h"

#include <assert.h>

class Vec2I
{
    friend Vec2I operator+( const Vec2I &, const Vec2I & );
    friend Vec2I operator*( const Vec2I &, const Vec2I & );
    friend Vec2I operator-( const Vec2I &, const Vec2I & );
    friend Vec2I operator-( const Vec2I & );
    friend Vec2I operator*( const Vec2I &, const Real );
    friend Vec2I operator*( const Real , const Vec2I & );
    friend Vec2I operator/( const Vec2I &, const Real );
    friend Vec2I operator/( const Vec2I &, const Vec2I & );
public:

    inline Vec2I()
    {
    }
    
    
    inline Vec2I( const S32 x, const S32 y )
    {
        SetValue( x, y );
    }
    
    inline Vec2I( const Real x, const Real y )
    {
        SetValue( ( S32 )x, ( S32 )y );
    }
    
    inline S32 &operator[]( const U8 axis )
    {
        return mValues[ axis ];
    }
    
    inline const S32 &operator[]( const U8 axis ) const
    {
        return mValues[ axis ];
    }
    
    inline void SetX( const S32 &val )
    {
        mValues[0] = val;
    }
    
    inline S32 GetX() const
    {
        return mValues[0];
    }
    
    inline void SetY( const S32 &val )
    {
        mValues[1] = val;
    }
    
    inline S32 GetY() const
    {
        return mValues[1];
    }
    
    inline void SetValue( const S32 &x, const S32 &y )
    {
        mValues[0] = x;
        mValues[1] = y;
    }
    
    inline void Clear()
    {
        SetValue( 0, 0 );
    }
    
    static inline Vec2I GetZero()
    {
        return Vec2I( 0, 0 );
    }
    
    static inline Vec2I GetOne()
    {
        return Vec2I( 1, 1 );
    }
    
    static inline Vec2I GetDown()
    {
        return Vec2I( 0, -1 );
    }
    
    static inline Vec2I GetUp()
    {
        return Vec2I( 0, 1 );
    }
    
    static inline Vec2I GetLeft()
    {
        return Vec2I( -1, 0 );
    }
    
    static inline Vec2I GetRight()
    {
        return Vec2I( 1, 0 );
    }

private:

    S32 mValues[2];

};

inline Vec2I operator+( const Vec2I &v1, const Vec2I &v2 )
{
    return Vec2I( v1.mValues[0] + v2.mValues[0],
                  v1.mValues[1] + v2.mValues[1] );
}

inline Vec2I operator*( const Vec2I &v1, const Vec2I &v2 )
{
    return Vec2I( v1.mValues[0] * v2.mValues[0],
                  v1.mValues[1] * v2.mValues[1] );
}

inline Vec2I operator-( const Vec2I &v1, const Vec2I &v2 )
{
    return Vec2I( v1.mValues[0] - v2.mValues[0],
                  v1.mValues[1] - v2.mValues[1] );
}

inline Vec2I operator-( const Vec2I &v )
{
    return Vec2I( -v.mValues[0], -v.mValues[1] );
}

inline Vec2I operator*( const Vec2I &v, const Real s )
{
    return Vec2I( v.mValues[0] * s, v.mValues[1] * s );
}

inline Vec2I operator*( const Real s, const Vec2I &v )
{
    return v * s;
}

inline Vec2I operator/( const Vec2I &v, const Real s )
{
    assert( s != 0.0f );

    return v * ( Real( 1.0 ) / s );
}

inline Vec2I operator/( const Vec2I &v1, const Vec2I &v2 )
{
    return Vec2I( v1.mValues[0] / v2.mValues[0],
                  v1.mValues[1] / v2.mValues[1] );
}
*/

#endif