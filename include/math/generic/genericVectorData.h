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
#ifndef __GENERIC_VECTOR_DATA_H__
#define __GENERIC_VECTOR_DATA_H__

#include "math/types.h"
#include "math/config.h"
#include "math/scalar/mathf.h"

#include <assert.h>

#pragma warning( disable : 4201 )

BEGIN_MATH_NAMESPACE

//
// Generic data construct
//
template< class TYPE, U32 N >
class GenericVectorData
{
protected:
    TYPE mValues[N];  
};

//specific unions
template< class TYPE >
class GenericVectorData< TYPE, 2 >
{
public:

    FORCE_INLINE void SetX( const TYPE p1 )
    {
        mValues[0] = p1;
    }
    
    FORCE_INLINE TYPE GetX() const
    {
        return mValues[0];
    }
    
    FORCE_INLINE void SetY( const TYPE p1 )
    {
        mValues[1] = p1;
    }
    
    FORCE_INLINE TYPE GetY() const
    {
        return mValues[1];
    }
    
    FORCE_INLINE void SetValues( const TYPE &p1, const TYPE &p2 )
    {
        mValues[0] = p1;
        mValues[1] = p2;
    }
    
    union 
    {
        TYPE mValues[2];
        struct { TYPE x, y; };
        struct { TYPE r, g; };
    };
};

template< class TYPE>
class GenericVectorData< TYPE, 3 >
{
public:
    
    FORCE_INLINE void SetX( const TYPE p1 )
    {
        mValues[0] = p1;
    }
    
    FORCE_INLINE TYPE GetX() const
    {
        return mValues[0];
    }
    
    FORCE_INLINE void SetY( const TYPE p1 )
    {
        mValues[1] = p1;
    }
    
    FORCE_INLINE TYPE GetY() const
    {
        return mValues[1];
    }
    
    FORCE_INLINE void SetZ( const TYPE p1 )
    {
        mValues[2] = p1;
    }
    
    FORCE_INLINE TYPE GetZ() const
    {
        return mValues[2];
    }
    
    FORCE_INLINE void SetValues( const TYPE &p1, const TYPE &p2, const TYPE &p3 )
    {
        mValues[0] = p1;
        mValues[1] = p2;
        mValues[2] = p3;
    }
    
    union 
    {
        TYPE mValues[3];
        struct { TYPE x, y, z; };
        struct { TYPE r, g, b; };
        GenericVectorData< TYPE,2 > xy;
        GenericVectorData< TYPE,2 > rg;
    };
};

template< class TYPE>
class GenericVectorData< TYPE, 4 >
{
public:

    FORCE_INLINE void SetX( const TYPE p1)
    {
        mValues[0] = p1;
    }
    
    FORCE_INLINE TYPE GetX() const
    {
        return mValues[0];
    }
    
    FORCE_INLINE void SetY( const TYPE p1)
    {
        mValues[1] = p1;
    }
    
    FORCE_INLINE TYPE GetY() const
    {
        return mValues[1];
    }
    
    FORCE_INLINE void SetZ( const TYPE p1)
    {
        mValues[2] = p1;
    }
    
    FORCE_INLINE TYPE GetZ() const
    {
        return mValues[2];
    }
    
    FORCE_INLINE void SetW( const TYPE p1)
    {
        mValues[3] = p1;
    }
    
    FORCE_INLINE TYPE GetW() const
    {
        return mValues[3];
    }
    
    FORCE_INLINE void SetValues( const TYPE &p1, const TYPE &p2, const TYPE &p3, const TYPE &p4 )
    {
        mValues[0] = p1;
        mValues[1] = p2;
        mValues[2] = p3;
        mValues[3] = p4;
    }
    
    union 
    {
        TYPE mValues[4];
        struct { TYPE x, y, z, w; };
        struct { TYPE r, g, b, a; };
        GenericVectorData< TYPE,2 > xy;
        GenericVectorData< TYPE,2 > rg;
        GenericVectorData< TYPE,3 > xyz;
        GenericVectorData< TYPE,3 > rgb;
    };
};


END_MATH_NAMESPACE

#endif