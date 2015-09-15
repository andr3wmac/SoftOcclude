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

    inline void SetX( const TYPE p1 )
    {
        mValues[0] = p1;
    }
    
    inline TYPE GetX() const
    {
        return mValues[0];
    }
    
    inline void SetY( const TYPE p1 )
    {
        mValues[1] = p1;
    }
    
    inline TYPE GetY() const
    {
        return mValues[1];
    }
    
    inline void SetValues( const TYPE &p1, const TYPE &p2 )
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
    
    inline void SetX( const TYPE p1 )
    {
        mValues[0] = p1;
    }
    
    inline TYPE GetX() const
    {
        return mValues[0];
    }
    
    inline void SetY( const TYPE p1 )
    {
        mValues[1] = p1;
    }
    
    inline TYPE GetY() const
    {
        return mValues[1];
    }
    
    inline void SetZ( const TYPE p1 )
    {
        mValues[2] = p1;
    }
    
    inline TYPE GetZ() const
    {
        return mValues[2];
    }
    
    inline void SetValues( const TYPE &p1, const TYPE &p2, const TYPE &p3 )
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

    inline void SetX( const TYPE p1)
    {
        mValues[0] = p1;
    }
    
    inline TYPE GetX() const
    {
        return mValues[0];
    }
    
    inline void SetY( const TYPE p1)
    {
        mValues[1] = p1;
    }
    
    inline TYPE GetY() const
    {
        return mValues[1];
    }
    
    inline void SetZ( const TYPE p1)
    {
        mValues[2] = p1;
    }
    
    inline TYPE GetZ() const
    {
        return mValues[2];
    }
    
    inline void SetW( const TYPE p1)
    {
        mValues[3] = p1;
    }
    
    inline TYPE GetW() const
    {
        return mValues[3];
    }
    
    inline void SetValues( const TYPE &p1, const TYPE &p2, const TYPE &p3, const TYPE &p4 )
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