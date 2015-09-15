#pragma once
#ifndef __SOAVEC3_H__
#define __SOAVEC3_H__

#include <vector>
#include <iostream>

#include "math/types.h"

#include "memory/alignedAllocator.h"

#include "math/scalar/vec3.h"

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