#pragma once
#ifndef __MESH_RASTERIZER_H__
#define __MESH_RASTERIZER_H__

#include "core/common/types.h"

#include "math/simd/simdTraits.h"
#include "math/simd/avx.h"
#include "math/simd/sse2.h"
#include "math/simd/sse41.h"
#include "math/simd/fpu.h"

#include "math/util/memory/stackAlign.h"

#include "depthBuffer.h"

template< typename SimdTraitsReal, typename SimdTraitsInt >
struct MeshRasterizer
{
    typedef typename SimdTraitsReal::vec_type  simd_vec;
    typedef typename SimdTraitsReal::bool_type simd_bool;
    typedef typename SimdTraitsInt::vec_type   simd_veci;
    typedef typename SimdTraitsInt::bool_type  simd_booli;
    //
    // Helper
    //

    class SimdVertexSet
    {
    public:

        inline SimdVertexSet()
        {
        }

        inline SimdVertexSet( F32 *tx, F32 *ty, F32 *tz )
        {
            simd_vec tempX, tempY;

            tempX.LoadAligned( tx );
            tempY.LoadAligned( ty );

            x = simd_veci(tempX);
            y = simd_veci(tempY);
            z.LoadAligned( tz );
        }

        simd_veci x;
        simd_veci y;
        simd_vec  z;
    };

    class SimdTriangleSet
    {
    public:

        inline SimdTriangleSet( const SimdVertexSet &v1,
                                const SimdVertexSet &v2,
                                const SimdVertexSet &v3 )
        {
            mVertices[0] = v1;
            mVertices[1] = v2;
            mVertices[2] = v3;
        }

        const SimdVertexSet &operator[]( U32 i ) const
        {
            assert( i < 3 );

            return mVertices[i];
        }

    private:

        SimdVertexSet mVertices[3];
    };

    class SimdEdge
    {
    public:

        inline SimdEdge(const SimdVertexSet &v1, const SimdVertexSet &v2)
        {
            a = v1.y - v2.y;
            b = v2.x - v1.x;
            c = v1.x * v2.y - v2.x * v1.y;

            xStride = a << 1;
            yStride = b << 1;
        }

        inline simd_veci EdgeFunction(const simd_veci &x, const simd_veci &y) const
        {
            return (a * x) + (b * y) + c;
        }

        simd_veci a;
        simd_veci b;
        simd_veci c;

        simd_veci xStride = a << 1;
        simd_veci yStride = b << 1;
    };

    //
    // Functions
    //
   
    template< U32 TriangleIndex >
    static inline void RasterizeTriangle(
        const DepthBufferTile &tile,
        const simd_veci &texelPatternX,
        const simd_veci &texelPatternY,
        const SimdTriangleSet &triangles,
        const SimdEdge &edge1,
        const SimdEdge &edge2,
        const SimdEdge &edge3,
        const simd_vec &triangleAreaRcp,
        const simd_veci &startFillX,
        const simd_veci &endFillX,
        const simd_veci &startFillY,
        const simd_veci &endFillY,
        SimdDepthBuffer &depthBuffer )
    {
        simd_vec tri_z_0 = triangles[0].z.BroadcastIndex<TriangleIndex>();
        simd_vec tri_z_1 = triangles[1].z.BroadcastIndex<TriangleIndex>();
        simd_vec tri_z_2 = triangles[2].z.BroadcastIndex<TriangleIndex>();
        
        const simd_vec triAreaRcp = triangleAreaRcp.BroadcastIndex<TriangleIndex>();
        
        tri_z_0 *= triAreaRcp;
        tri_z_1 *= triAreaRcp;
        tri_z_2 *= triAreaRcp;
        
        const simd_veci tri_a0 = edge1.a.BroadcastIndex<TriangleIndex>();
        const simd_veci tri_a1 = edge2.a.BroadcastIndex<TriangleIndex>();
        const simd_veci tri_a2 = edge3.a.BroadcastIndex<TriangleIndex>();

        const simd_veci tri_b0 = edge1.b.BroadcastIndex<TriangleIndex>();
        const simd_veci tri_b1 = edge2.b.BroadcastIndex<TriangleIndex>();
        const simd_veci tri_b2 = edge3.b.BroadcastIndex<TriangleIndex>();

        const simd_veci tri_c0 = edge1.c.BroadcastIndex<TriangleIndex>();
        const simd_veci tri_c1 = edge2.c.BroadcastIndex<TriangleIndex>();
        const simd_veci tri_c2 = edge3.c.BroadcastIndex<TriangleIndex>();

        const simd_veci tri_a0_stride = edge1.xStride.BroadcastIndex<TriangleIndex>();
        const simd_veci tri_a1_stride = edge2.xStride.BroadcastIndex<TriangleIndex>();
        const simd_veci tri_a2_stride = edge3.xStride.BroadcastIndex<TriangleIndex>();

        const S32 ac_min_x = startFillX.ExtractIndex<TriangleIndex>();
        const S32 ac_min_y = startFillY.ExtractIndex<TriangleIndex>();
        const S32 ac_max_x = endFillX.ExtractIndex<TriangleIndex>();
        const S32 ac_max_y = endFillY.ExtractIndex<TriangleIndex>();

        const simd_veci tri_fillX = simd_veci(ac_min_x) + texelPatternX;

        const simd_veci tri_b0_stride = edge1.yStride.BroadcastIndex<TriangleIndex>();
        const simd_veci tri_b1_stride = edge2.yStride.BroadcastIndex<TriangleIndex>();
        const simd_veci tri_b2_stride = edge3.yStride.BroadcastIndex<TriangleIndex>();

        const simd_veci tri_fillY = simd_veci(ac_min_y) + texelPatternY;

        simd_veci w0_row = (tri_a0 * tri_fillX) + (tri_b0 * tri_fillY) + tri_c0;
        simd_veci w1_row = (tri_a1 * tri_fillX) + (tri_b1 * tri_fillY) + tri_c1;
        simd_veci w2_row = (tri_a2 * tri_fillX) + (tri_b2 * tri_fillY) + tri_c2;

        U32 rowStoreStride = tile.tileSizeX * ac_min_y * 4 + ac_min_x * 4;
        
        // Progress
        // Iterate y dimension
        for (U32 y = ac_min_y, yend = ac_max_y; y < yend; ++y)
        {
            U32 storeStride = rowStoreStride;

            simd_veci w0 = w0_row;
            simd_veci w1 = w1_row;
            simd_veci w2 = w2_row;

            for (U32 x = ac_min_x, xend = ac_max_x; x < xend; ++x)
            {
                const simd_veci iCoverageMask = w0 | w1 | w2;

                if (iCoverageMask.IsEmpty())
                {
                    continue;
                }

                simd_vec texelDepth = simd_vec(w0) * tri_z_0 + simd_vec(w1) * tri_z_1 + simd_vec(w2) * tri_z_2;

                F32 *depthLink = depthBuffer.GetDepthSequence(storeStride);

                simd_vec prevDepth;
                prevDepth.LoadAligned(depthLink);

                const simd_bool depthMask = (texelDepth >= prevDepth) & simd_bool(iCoverageMask);

                texelDepth = SIMD_Select(depthMask, prevDepth, texelDepth);
                texelDepth.StoreAligned(depthLink);

                storeStride += 4;
                w0 += tri_a0_stride;
                w1 += tri_a1_stride;
                w2 += tri_a2_stride;
            }

            rowStoreStride += 4 * tile.tileSizeX;
            w0_row += tri_b0_stride;
            w1_row += tri_b1_stride;
            w2_row += tri_b2_stride;
        }

        // RECURSIVE
        RasterizeTriangle< TriangleIndex + 1 >(tile, texelPatternX, texelPatternY, triangles,
                                               edge1, edge2, edge3, triangleAreaRcp,
                                               startFillX, endFillX,
                                               startFillY, endFillY,
                                               depthBuffer);
    }

    template<>
    static inline void RasterizeTriangle< SimdTraitsReal::width >
        (
            const DepthBufferTile &tile,
            const simd_veci &texelPatternX,
            const simd_veci &texelPatternY,
            const SimdTriangleSet &triangles,
            const SimdEdge &edge1,
            const SimdEdge &edge2,
            const SimdEdge &edge3,
            const simd_vec &triangleAreaRcp,
            const simd_veci &startFillX,
            const simd_veci &endFillX,
            const simd_veci &startFillY,
            const simd_veci &endFillY,
            SimdDepthBuffer &depthBuffer)
    {

    }

    static void RasterizeToDepthBuffer( const DepthBufferTile &tile,
                                        const simd_veci &texelPatternX,
                                        const simd_veci &texelPatternY,
                                        SimdDepthBuffer &depthBuffer,
                                        const SimdTriangleSet &triangles )
    {
        //
        // Process simd_width triangles
        //

        SimdEdge edge1(triangles[1], triangles[2]);
        SimdEdge edge2(triangles[2], triangles[0]);
        SimdEdge edge3(triangles[0], triangles[1]);

        simd_veci triangleArea = edge1.EdgeFunction(triangles[0].x, triangles[0].y);
        simd_vec  triangleAreaRcp = SIMD_Rcp( simd_vec( triangleArea ) );
      
        simd_veci startFillX = triangles[0].x.Min(triangles[1].x.Min(triangles[2].x));
        startFillX = startFillX.Max(simd_veci(tile.xStart));

        simd_veci endFillX = triangles[0].x.Max(triangles[1].x.Max(triangles[2].x));
        endFillX = endFillX.Min(simd_veci(tile.xEnd));

        simd_veci startFillY = triangles[0].y.Min(triangles[1].y.Min(triangles[2].y));
        startFillY = startFillY.Max(simd_veci(tile.yStart));

        simd_veci endFillY = triangles[0].y.Max(triangles[1].y.Max(triangles[2].y));
        endFillY = endFillY.Min(simd_veci(tile.yEnd));

        //
        // Per triangle process
        //
        RasterizeTriangle< 0 >( tile, texelPatternX, texelPatternY, triangles, 
                                edge1, edge2, edge3, triangleAreaRcp, 
                                startFillX, endFillX, 
                                startFillY, endFillY,  
                                depthBuffer);
        
    }

    static void RasterizeTileData( U32 tileShiftX, U32 tileShiftY,
                                   U32 tileSizeX, U32 tileSizeY,
                                   SimdDepthBuffer &depthBuffer )
    {

        F32 StackAlign( 64 ) tv1_x[4] = { 26.613068, 126.613068, 26.613068, 126.613068 };
        F32 StackAlign( 64 ) tv1_y[4] = { 12.613052, 12.613052, 112.613052, 12.613052 };
        F32 StackAlign( 64 ) tv1_z[4] = { 0.3, 0.4, 0.5, 0.6 };

        F32 StackAlign( 64 ) tv2_x[4] = { 37.386932, 137.386932, 37.386932, 137.386932 };
        F32 StackAlign( 64 ) tv2_y[4] = { 23.386948, 23.386948, 123.386948, 23.386948 };
        F32 StackAlign( 64 ) tv2_z[4] = { 0.4, 0.5, 0.6, 0.7 };

        F32 StackAlign( 64 ) tv3_x[4] = { 26.613068, 126.613068, 26.613068, 126.613068 };
        F32 StackAlign( 64 ) tv3_y[4] = { 23.386948, 23.386948, 123.386948, 23.386948 };
        F32 StackAlign( 64 ) tv3_z[4] = { 0.8, 0.9, 1.0, 1.1 };

        SimdTriangleSet triangles(
            SimdVertexSet( tv1_x, tv1_y, tv1_z ),
            SimdVertexSet( tv2_x, tv2_y, tv2_z ),
            SimdVertexSet( tv3_x, tv3_y, tv3_z )
        );

        simd_veci texelPatternX, texelPatternY;

        S32 StackAlign( 64 ) patternX[4] = { 0, 1, 0, 1 };
        S32 StackAlign( 64 ) patternY[4] = { 0, 0, 1, 1 };

        texelPatternX.LoadAligned( patternX );
        texelPatternY.LoadAligned( patternY );

        for (volatile U32 i = 0; i < 1000000; ++i)
        {
            RasterizeToDepthBuffer(DepthBufferTile( 400, 300,
                                                    0, 300,
                                                    0, 300),
                                                    texelPatternX,
                                                    texelPatternY,
                                                    depthBuffer, triangles);
        }
        
    }



};

#endif