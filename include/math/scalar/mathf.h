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

#ifndef __MATHF_H__
#define __MATHF_H__

#include "math/types.h"
#include "math/config.h"

#include <assert.h>
#include <cfloat>
#include <cmath>
#include <cstdlib>

#define _USE_MATH_DEFINES
#include <math.h>

BEGIN_MATH_NAMESPACE

namespace Mathf
{
    inline Real GetEpsilon()
    {
#ifdef  REAL_PRECISION_DOUBLE
        return DBL_EPSILON;
#else
        return FLT_EPSILON;
#endif
    }
    
    inline Real GetPi()
    {
        return ( Real )M_PI;
    }
    
    inline Real Get2Pi()
    {
        return ( Real )M_PI_2;
    }
    
    inline Real GetSqrt2()
    {
        return ( Real )M_SQRT2;
    }
    
    inline Real GetE()
    {
        return ( Real )M_E;
    }
    
    inline Real GetLog2E()
    {
        return ( Real )M_LOG2E;
    }
    
    inline Real GetLog10E()
    {
        return ( Real )M_LOG10E;
    }
    
    inline Real GetLn2()
    {
        return ( Real )M_LN2;
    }
    
    inline Real GetLn10()
    {
        return ( Real )M_LN10;
    }
    
    inline Real Acos( const Real f )
    {
        assert( f >= -1.0f && f <= 1.0f );
        
#ifdef REAL_PRECISION_DOUBLE
        return acos( f );
#else
        return acosf( f );
#endif
    }
    
    inline Real Asin( const Real f )
    {
        assert( f >= -1.0f && f <= 1.0f );
    
#ifdef REAL_PRECISION_DOUBLE
        return asin( f );
#else
        return asinf( f );
#endif
    }
    
    inline Real Atan( const Real f )
    {
#ifdef REAL_PRECISION_DOUBLE
        return atan( f );
#else
        return atanf( f );
#endif
    }
    
    inline Real Atan2( const Real x, const Real y )
    {
#ifdef REAL_PRECISION_DOUBLE
        return atan2( x, y );
#else
        return atan2f( x, y );
#endif
    }
    
    inline Real Cos( const Real f )
    {
#ifdef REAL_PRECISION_DOUBLE
        return cos( f );
#else
        return cosf( f );
#endif
    }
    
    inline Real Sin( const Real f )
    {
#ifdef REAL_PRECISION_DOUBLE
        return sin( f );
#else
        return sinf( f );
#endif
    }
    
    inline Real Tan( const Real f )
    {
#ifdef REAL_PRECISION_DOUBLE
        return tan( f );
#else
        return tanf( f );
#endif
    }
    
    inline Real Exp( const Real f )
    {
#ifdef REAL_PRECISION_DOUBLE
        return exp( f );
#else
        return expf( f );
#endif
    }
    
    inline Real Ceil( const Real f )
    {
#ifdef REAL_PRECISION_DOUBLE
        return ceil( f );
#else
        return ceilf( f );
#endif
    }
    
    inline Real Floor( const Real f )
    {
#ifdef REAL_PRECISION_DOUBLE
        return floor( f );
#else
        return floorf( f );
#endif
    }
    
    inline Real Round( const Real f )
    {
        return f >= 0.0 ? f + 0.5f : ( ( f - ( Real )( S32 )f ) <= -0.5 ? f : f - 0.5f );
    }
    
    inline Real Log( const Real f )
    {
        assert( f > 0.0f );
    
#ifdef REAL_PRECISION_DOUBLE
        return log( f );
#else
        return logf( f );
#endif
    }
    
    inline Real Log10( const Real f )
    {
        assert( f > 0.0f );   
#ifdef REAL_PRECISION_DOUBLE
        return log10( f );
#else
        return log10f( f );
#endif
    }
    
    inline Real Pow( const Real base, const Real exp )
    {
#ifdef REAL_PRECISION_DOUBLE
        return pow( base, exp );
#else
        return powf( base, exp );
#endif
    }
    
    inline Real Clamp( const Real f, const Real min, const Real max )
    {
        return f < min ? min : ( f > max ? max : f );
    }
    
    inline Real RadToDeg( const Real f )
    {
        return f * 180.0f / GetPi();
    }
    
    inline Real DegToRad( const Real f )
    {
        return f * GetPi() / 180.0f;
    }
    
    inline Real Lerp( const Real a, const Real b, const Real t )
    {
        return a + ( b - a ) * t;
    }
    
    inline bool IsPow2( const U32 n )
    {
        return ( !( n & ( n - 1 ) ) && n );
    }
    
    inline U32 NextPow2( const U32 x )
    {
        U32 y = x - 1;
        y |= y >> 1;
        y |= y >> 2;
        y |= y >> 4;
        y |= y >> 8;
        y |= y >> 16;
        return ++y;
    }
    
    inline Real Square( Real a )
    {
        return a * a;
    }

    inline Real Sqrt( const Real f )
    {
        assert( f >= 0.0f );
#ifdef REAL_PRECISION_DOUBLE
        return sqrt( f );
#else
        return sqrtf( f );
#endif
    }
    
    inline Real Abs( const Real f )
    {
#ifdef REAL_PRECISION_DOUBLE
        return std::abs( f );
#else
        return fabsf( f );
#endif
    }
    
    inline U32 Abs( const S32 f )
    {
        return abs( ( int )f );
    }
    
    inline S32 Rint( Real x )
    {
#ifdef _WIN32
        return ( S32 )( x + 0.5 );
#else
        return ( S32 )rint( x );
#endif
    }
  
    inline bool Equal( const Real a, const Real b )
    {
#ifdef REAL_PRECISION_DOUBLE
        return std::abs( a - b ) < GetEpsilon();
#else
        return fabs( a - b ) < GetEpsilon();
#endif
    }

    template< typename tT >
    inline tT GetMin( const tT &a, const tT &b )
    {
        return a < b ? a : b;
    }

    template< typename tT >
    inline tT GetMax( const tT &a, const tT &b )
    {
        return a > b ? a : b;
    }

    template< typename tT >
    inline tT Rcp( const tT &_a )
    {
        return (tT)1.0 / _a;
    }
}

#endif