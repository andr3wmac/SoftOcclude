#pragma once
#ifndef __VECTORIZE_H__
#define __VECTORIZE_H__

//align to cache line
#define SIMD_ALIGNMENT 64

//
//define simd widths and support
//

//based on the really nice article: http://jmabille.github.io/blog/2014/10/25/writing-c-plus-plus-wrappers-for-simd-intrinsics-5/
#ifdef _VECTORIZE

//
// Read what static instruction set we have available
//

#ifdef _PREBAKE_SIMD_INSTRUCTION_SET

#ifndef SIMD_INSTRUCTION_SET

#ifdef __AVX2__
#define SIMD_INSTRUCTION_SET 7

#elif  __AVX__
#define SIMD_INSTRUCTION_SET 6

#elif __SSE4_2__
#define SIMD_INSTRUCTION_SET 5

#elif ( __SSE4_1__ || ( _M_IX86_FP == 2 ) )
#define SIMD_INSTRUCTION_SET 4

#elif  __SSE3__
#define SIMD_INSTRUCTION_SET 3

#elif ( __SSE2__ || _M_X86 )
#define SIMD_INSTRUCTION_SET 2

#elif  __SSE__
#define SIMD_INSTRUCTION_SET 1

#else
#define SIMD_INSTRUCTION_SET 0

#endif

#endif // SIMD_INSTRUCTION_SET

#else
#define SIMD_INSTRUCTION_SET 0 // fallback to FP
#endif // _VECTORIZE

//
// Based on the set make defines and include
//

// If AVX2
#if SIMD_INSTRUCTION_SET == 7
//#ifdef __GNUC__
//    #include <x86intrin.h>
//#else

#include <immintrin.h>
//#endif
// If AVX
#elif SIMD_INSTRUCTION_SET == 6
#include <immintrin.h>
// If SSE4.2
#elif SIMD_INSTRUCTION_SET == 5
#include <nmmintrin.h>

// If SSE4.1
#elif SIMD_INSTRUCTION_SET == 4
#include <smmintrin.h>

// If SSE3
#elif SIMD_INSTRUCTION_SET == 3
#include <pmmintrin.h>

// If SSE2
#elif SIMD_INSTRUCTION_SET == 2
#include <emmintrin.h>

// If SSE
#elif SIMD_INSTRUCTION_SET == 1
#include <xmmintrin.h>
#endif // SIMD_INSTRUCTION_SET

#endif //_PREBAKE_SIMD_INSTRUCTION_SET

#endif // __VECTORIZE_H__