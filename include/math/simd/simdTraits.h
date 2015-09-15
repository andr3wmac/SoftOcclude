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