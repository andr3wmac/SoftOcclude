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
#ifndef __SIMD_STATUS_H__
#define __SIMD_STATUS_H__

#include "math/types.h"

//
// CPUID
//

#include <vector>
#include <array>
#include <string>
#include <bitset>

#ifdef _WIN32
#define GetCpuID __cpuid
#else
#ifndef __LP64__
#error currently cpuid is only supported for x86_64 platforms!
#endif
#define GetCpuID(iarray, func)\
	    __asm__ __volatile__ ( \
       "cpuid":\
	   "=a" (iarray[0]),\
       "=b" (iarray[1]),\
       "=c" (iarray[2]),\
       "=d" (iarray[3]) :\
       "a" (func));
#endif

//
// Status
//

/*
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
*/

class SimdStatus
{
public:

    inline SimdStatus() : mIntelCpu( false ), mAmdCpu( false )
    {
        CaptureCpuInfo();
        DefineVendor();
    }

    inline bool SupportsSSE() const
    {
        return mFunctionLevel1_DINT[25]; 
    }	
    
    inline bool SupportsSSE2() const
    {
        return mFunctionLevel1_DINT[26]; 
    }	
    
    inline bool SupportsSSE3() const
    {
        return mFunctionLevel1_CINT[9]; 
    }	

    inline bool SupportsSSE4_1() const
    {
        return mFunctionLevel1_CINT[19]; 
    }	
    
    inline bool SupportsSSE4_2() const
    {
        return mFunctionLevel1_CINT[20]; 
    }
    
    inline bool SupportsAVX() const
    {
        return mFunctionLevel1_CINT[28]; 
    }

    inline bool SupportsFMA() const
    {
        return mFunctionLevel1_CINT[12]; 
    }
    
    inline U32 SIMDLevel() const
    {
        U32 level = 0;
        
        if ( SupportsSSE() )   { level++; }
        if ( SupportsSSE2() )  { level++; }
        if ( SupportsSSE3() )  { level++; }
        if ( SupportsSSE4_1() ){ level++; }
        if ( SupportsSSE4_2() ){ level++; }
        if ( SupportsAVX() )   { level++; }
        if ( SupportsFMA() )   { level++; }
        
        return level;
    }
    
    inline const std::string &Vendor() const
    {
        return mVendor;
    }

private:

    inline void CaptureCpuInfo()
    {
        std::array<int, 4> cpuid;

        GetCpuID(cpuid.data(), 0);
        
        const S32 ilevels = cpuid[0];
        
        for ( S32 i=0; i < ilevels; ++i )
        {
            GetCpuID(cpuid.data(), i);
            mFuncLevels.push_back(cpuid);
        }
    }
    
    inline void DefineVendor()
    {
        char vendor[32];
        memset( vendor, 0, sizeof(vendor) );
        memcpy( vendor, (char*) &mFuncLevels[0][1], sizeof( S32 ) );
        memcpy( vendor + 4, (char*) &mFuncLevels[0][3], sizeof( S32 ) );
        memcpy( vendor + 8, (char*) &mFuncLevels[0][2], sizeof( S32 ) );
        
        mVendor = std::string( vendor );
        
        if ( mVendor == "AuthenticAMD" )
        {
            mAmdCpu = true;
        }
        
        if ( mVendor == "GenuineIntel" )
        {
            mIntelCpu = true;
        }
        
        if ( mFuncLevels.size() >= 1 )
        {
            mFunctionLevel1_CINT = mFuncLevels[1][2];
            mFunctionLevel1_DINT = mFuncLevels[1][3];
        }
        
        if ( mFuncLevels.size() >= 7 ) 
        {
            mFunctionLevel7_BINT = mFuncLevels[7][1];
            mFunctionLevel7_CINT = mFuncLevels[7][2];
        }
    }
    
    bool mIntelCpu;
    bool mAmdCpu;
    
    std::string mVendor;
    
    std::bitset<32> mFunctionLevel1_CINT;
    std::bitset<32> mFunctionLevel1_DINT;
    std::bitset<32> mFunctionLevel7_BINT;
    std::bitset<32> mFunctionLevel7_CINT;
    std::vector< std::array<S32, 4> > mFuncLevels;
};

#endif