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
#ifndef __SOAVEC3_H__
#define __SOAVEC3_H__

#include <vector>
#include <iostream>

#include "math/types.h"

#include "math/util/memory/alignedAllocator.h"

#include "math/scalar/vec3.h"

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

#include "math/simd/simdTraits.h"
#include "math/simd/vectorize.h"

namespace LibStruct
{
    class SOAVec3
    {
    public:
    
        SOAVec3 () : mSize( 0 )
        {}
    
        void Resize( size_t targetSize )
        {
            x.clear();
            y.clear();
            z.clear();
            
            size_t simdRest = ( SimdTraits<Real>::width - targetSize % SimdTraits<Real>::width );
            
            mSize = targetSize;
            
            // full fill
            x.resize( targetSize + simdRest, 0.0 );
            y.resize( targetSize + simdRest, 0.0 );
            z.resize( targetSize + simdRest, 0.0 );
            
            // then resize back, which is one internal index switch,
            // vectors are by standard never releasing extra mem
            x.resize( targetSize );
            y.resize( targetSize );
            z.resize( targetSize );
        }
        
        size_t Size() const
        {
            return mSize;
        }
        
        void Debug()
        {
            for ( size_t i=0; i < x.size(); ++i )
            {
                std::cout << " DEBUG SOAVec3 " << x[i] << " " << y[i] << " " << z[i] << std::endl;
            }
        }
        
        std::vector< Real, AlignedAllocator< Real, SIMD_ALIGNMENT > > &GetX() 
        {
            return x;
        }
        
        const std::vector< Real, AlignedAllocator< Real, SIMD_ALIGNMENT > > &GetX() const
        {
            return x;
        }
        
        Real GetX( size_t index ) const
        {
            return x[index];
        }
        
        std::vector< Real, AlignedAllocator< Real, SIMD_ALIGNMENT > > &GetY()
        {
            return y;
        }
        
        const std::vector< Real, AlignedAllocator< Real, SIMD_ALIGNMENT > > &GetY() const
        {
            return y;
        }
        
        Real GetY( size_t index ) const
        {
            return y[index];
        }
        
        std::vector< Real, AlignedAllocator< Real, SIMD_ALIGNMENT > > &GetZ()
        {
            return z;
        }
        
        const std::vector< Real, AlignedAllocator< Real, SIMD_ALIGNMENT > > &GetZ() const
        {
            return z;
        }
        
        Real GetZ( size_t index ) const
        {
            return z[index];
        }
        
        void SetUnaligned( size_t index, const Vec3 &val )
        {
            x[index] = val[0];
            y[index] = val[1];
            z[index] = val[2];
        }	
        
    private:
       
        size_t mSize;
       
        std::vector< Real, AlignedAllocator< Real, SIMD_ALIGNMENT > > x;
        std::vector< Real, AlignedAllocator< Real, SIMD_ALIGNMENT > > y;
        std::vector< Real, AlignedAllocator< Real, SIMD_ALIGNMENT > > z;
    };
}

#endif