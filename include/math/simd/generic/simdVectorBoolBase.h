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
#ifndef __SIMD_VECTOR_BOOL_BASE_H__
#define __SIMD_VECTOR_BOOL_BASE_H__

// big thanks to http://jmabille.github.io/blog/2014/10/10/writing-c-plus-plus-wrappers-for-simd-intrinsics-3/
// for the idea's on how to nicely wrap

#include "math/config.h"

template< typename TYPE >
class SimdVectorBoolBase
{
public:
   
    //downcast
    FORCE_INLINE TYPE& operator()()
    {
        return *static_cast< TYPE* >( this );
    }
    
    FORCE_INLINE const TYPE& operator()() const
    {
        return *static_cast< const TYPE* >( this );
    }
    
    //
    // handle all self operators here
    //
    FORCE_INLINE TYPE& operator&= ( const TYPE& rhs )
    {
        (*this)() = (*this)() && rhs;
        return (*this)();
    } 
    
    FORCE_INLINE TYPE& operator|= ( const TYPE& rhs )
    {
        (*this)() = (*this)() || rhs;
        return (*this)();
    }
    
    FORCE_INLINE TYPE& operator^= ( const TYPE& rhs )
    {
        (*this)() = (*this)() ^ rhs;
        return (*this)();
    } 
    
protected:

    FORCE_INLINE SimdVectorBoolBase(){}
    FORCE_INLINE ~SimdVectorBoolBase(){}
    	
    FORCE_INLINE SimdVectorBoolBase( const SimdVectorBoolBase & ){}
    FORCE_INLINE SimdVectorBoolBase( const SimdVectorBoolBase && ){}
    FORCE_INLINE SimdVectorBoolBase& operator= ( const SimdVectorBoolBase & ){ return *this; }
};

template< typename TYPE >
FORCE_INLINE TYPE operator&&( const SimdVectorBoolBase< TYPE > &lhs, const SimdVectorBoolBase< TYPE > &rhs )
{
    return lhs() & rhs();
}

template< typename TYPE >
FORCE_INLINE TYPE operator&&( const SimdVectorBoolBase< TYPE > &lhs, bool rhs )
{
    return lhs() & TYPE( rhs );
}

template< typename TYPE >
FORCE_INLINE TYPE operator&&(  bool lhs, const SimdVectorBoolBase< TYPE > &rhs )
{
    return TYPE( lhs ) & rhs();
}

template< typename TYPE >
FORCE_INLINE TYPE operator||( const SimdVectorBoolBase< TYPE > &lhs, const SimdVectorBoolBase< TYPE > &rhs )
{
    return lhs() | rhs();
}

template< typename TYPE >
FORCE_INLINE TYPE operator||( const SimdVectorBoolBase< TYPE > &lhs, bool rhs )
{
    return lhs() | TYPE( rhs );
}

template< typename TYPE >
FORCE_INLINE TYPE operator||(  bool lhs, const SimdVectorBoolBase< TYPE > &rhs )
{
    return TYPE( lhs ) | rhs();
}

template< typename TYPE >
FORCE_INLINE TYPE operator!( const SimdVectorBoolBase< TYPE > &rhs )
{
    return rhs() == TYPE( 0 );
}

#endif