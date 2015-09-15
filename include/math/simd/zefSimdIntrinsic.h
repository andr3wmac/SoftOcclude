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
#ifndef __SIMD_INTRINSIC_FUNCTIONS_H__
#define __SIMD_INTRINSIC_FUNCTIONS_H__

#include "math/types.h"

#include <assert.h>


template< class SIMD_TYPE, class INIT_TYPE, U8 N, U8 M >
class SimdMatNM
{
public:
    
    SimdMatNM()
    {
    }
    
    SimdMatNM( class INIT_TYPE &init_t )
    {
        for ( U8 i=0; i < N; ++i )
        {
            for ( U8 j=0; j < M; ++j )
            {
                mValues[ i*j + j ] = SIMD_TYPE( init_t[i][j] );
            }
        }
    }
    
    const SIMD_TYPE &GetCell( U8 index )
    {
        assert( index < N*M );
        
        return mValues[index];
    }

private:    
    
    SIMD_TYPE mValues[ N*M ];
}


//To handle the functions for all available types
template< class SIMD_TYPE >
inline SIMD_TYPE Dot( const SIMD_TYPE &x1, const SIMD_TYPE &y1, const SIMD_TYPE &z1,
                      const SIMD_TYPE &x2, const SIMD_TYPE &y2, const SIMD_TYPE &z2 )
{
    return FMA_ADD( x1, x2, FMA_ADD( y1, y2, ( z1 * z2 )  ) );                          
}

template< class SIMD_TYPE >
inline void Cross( const SIMD_TYPE &x1, const SIMD_TYPE &y1, const SIMD_TYPE &z1,
                   const SIMD_TYPE &x2, const SIMD_TYPE &y2, const SIMD_TYPE &z2,
                   SIMD_TYPE &ox, SIMD_TYPE &oy, SIMD_TYPE &oz )
{
    ox = FMA_SUB( y1, z2, ( z1 * y2 ) );
    oy = FMA_SUB( z1, x2, ( x1 * z2 ) );
    oz = FMA_SUB( x1, y2, ( y1 * x2 ) ); 
}


#endif