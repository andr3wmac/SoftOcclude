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
#ifndef __SIMD_TRAITS_H__
#define __SIMD_TRAITS_H__

#include "math/simd/generic/simdBaseTraits.h"
#include "math/simd/vectorize.h"

//
// STATIC MECHANISM
// USE TO COMPILE THE INSTRUCTION SET INTO THE BINARY
// 

template< typename VALUE_TYPE >
struct SimdTraits;

#ifdef _PREBAKE_SIMD_INSTRUCTION_SET

// Fallback to scalar instructions
#if SIMD_INSTRUCTION_SET >= 6

#include "math/simd/avx/avx_vec8f.h"
#include "math/simd/avx/avx_vec4d.h"

template<>
struct SimdTraits< F32 > : public AvxSimdTraits< F32 >
{
    
};

template<>
struct SimdTraits< F64 > : public AvxSimdTraits< F64 >
{
    
};

#elif SIMD_INSTRUCTION_SET >= 4

#include "math/simd/sse4_1/sse4_1_vec4f.h"
#include "math/simd/sse4_1/sse4_1_vec2d.h"

template<>
struct SimdTraits< F32 > : public SSE4_1SimdTraits< F32 >
{
    
};

template<>
struct SimdTraits< F64 > : public SSE4_1SimdTraits< F64 >
{
    
};

#else

#include "math/simd/scalarFallback/scalar_vec4f.h"
#include "math/simd/scalarFallback/scalar_vec4d.h"

template<>
struct SimdTraits< F32 > : public FpuSimdTraits< F32 >
{
    
};

template<>
struct SimdTraits< F64 > : public FpuSimdTraits< F64 >
{
    
};

#endif //SIMD_INSTRUCTION_SET

#endif //_PREBAKE_SIMD_INSTRUCTION_SET

#endif