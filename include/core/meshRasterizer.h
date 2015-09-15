#pragma once
#ifndef __MESH_RASTERIZER_H__
#define __MESH_RASTERIZER_H__

#include "core/common/types.h"

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

        inline SimdVertexSet(F32 *x, F32 *y, F32 *z)
        {
            mValues[0].LoadAligned(x);
            mValues[1].LoadAligned(y);
            mValues[2].LoadAligned(z);
        }

        const simd_vec & operator[](U32 i) const
        {
            assert(i < 3);

            return mValues[i];
        }

    private:

        simd_vec mValues[3];
    };

    class SimdTriangleSet
    {
    public:

        inline SimdTriangleSet(const SimdVertexSet & v1,
            const SimdVertexSet & v2,
            const SimdVertexSet & v3)
        {
            mVertices[0] = v1;
            mVertices[1] = v2;
            mVertices[2] = v3;
        }

        const SimdVertexSet & operator[](U32 i) const
        {
            assert(i < 3);

            return mVertices[i];
        }

    private:

        SimdVertexSet mVertices[3];
    };

    //
    // Functions
    //

    static void RasterizeToDepthBuffer(const DepthBufferTile &tile,
        const simd_veci &pixelPatternX,
        const simd_veci &pixelPatternY,
        SimdDepthBuffer &depthBuffer,
        const SimdTriangleSet &triangles)
    {
        //
        // Process simd_width triangles
        //

        simd_veci iVertexXY[3][2];

        for (U32 i = 0; i < 3; ++i)
        {
            for (U32 j = 0; j < 2; ++j)
            {
                iVertexXY[i][j] = simd_veci(triangles[i][j]);
            }
        }

        simd_veci A0 = iVertexXY[1][1] - iVertexXY[2][1];
        simd_veci A1 = iVertexXY[2][1] - iVertexXY[0][1];
        simd_veci A2 = iVertexXY[0][1] - iVertexXY[1][1];

        simd_veci B0 = iVertexXY[2][0] - iVertexXY[1][0];
        simd_veci B1 = iVertexXY[0][0] - iVertexXY[2][0];
        simd_veci B2 = iVertexXY[1][0] - iVertexXY[0][0];

        simd_veci C0 = iVertexXY[1][0] * iVertexXY[2][1] - iVertexXY[2][0] * iVertexXY[1][1];
        simd_veci C1 = iVertexXY[2][0] * iVertexXY[0][1] - iVertexXY[0][0] * iVertexXY[2][1];
        simd_veci C2 = iVertexXY[0][0] * iVertexXY[1][1] - iVertexXY[1][0] * iVertexXY[0][1];

        simd_veci triangleArea = (A0 * iVertexXY[0][0]) + (B0 * iVertexXY[0][1]) + C0;
        simd_vec triangleArea_rcp = SIMD_Rcp(simd_vec(triangleArea));

        // 
        // Per triangle process
        //

        simd_veci startFillX = iVertexXY[0][0].Min(iVertexXY[1][0].Min(iVertexXY[2][0]));
        startFillX = startFillX.Max(simd_veci(tile.xStart));
        simd_veci endFillX = iVertexXY[0][0].Max(iVertexXY[1][0].Max(iVertexXY[2][0]));
        endFillX = endFillX.Max(simd_veci(tile.xEnd));

        simd_veci startFillY = iVertexXY[0][1].Min(iVertexXY[1][1].Min(iVertexXY[2][1]));
        startFillY = startFillY.Max(simd_veci(tile.yStart));
        simd_veci endFillY = iVertexXY[0][1].Max(iVertexXY[1][1].Max(iVertexXY[2][1]));
        endFillY = endFillY.Max(simd_veci(tile.yEnd));


        F32 StackAlign(SimdTraitsReal::alignment) stackDepth[3][SimdTraitsReal::width];
        triangles[0][2].StoreAligned(stackDepth[0]);
        triangles[1][2].StoreAligned(stackDepth[1]);
        triangles[2][2].StoreAligned(stackDepth[2]);

        F32 StackAlign(SimdTraitsReal::alignment) stackAreaRcp[SimdTraitsReal::width];
        triangleArea_rcp.StoreAligned(stackAreaRcp);

        S32 StackAlign(SimdTraitsReal::alignment) minMaxXY[2][2][SimdTraitsReal::width];
        startFillX.StoreAligned(minMaxXY[0][0]);
        endFillX.StoreAligned(minMaxXY[1][0]);
        startFillY.StoreAligned(minMaxXY[0][1]);
        endFillY.StoreAligned(minMaxXY[1][1]);

        S32 StackAlign(SimdTraitsReal::alignment) stackA[3][SimdTraitsReal::width];
        A0.StoreAligned(stackA[0]);
        A1.StoreAligned(stackA[1]);
        A2.StoreAligned(stackA[2]);

        S32 StackAlign(SimdTraitsReal::alignment) stackB[3][SimdTraitsReal::width];
        B0.StoreAligned(stackB[0]);
        B1.StoreAligned(stackB[1]);
        B2.StoreAligned(stackB[2]);

        S32 StackAlign(SimdTraitsReal::alignment) stackC[3][SimdTraitsReal::width];
        C0.StoreAligned(stackC[0]);
        C1.StoreAligned(stackC[1]);
        C2.StoreAligned(stackC[2]);

        for (U32 tri = 0; tri < SimdTraitsReal::width; ++tri)
        {
            simd_vec tri_z_0(stackDepth[0][tri]);
            simd_vec tri_z_1(stackDepth[1][tri]);
            simd_vec tri_z_2(stackDepth[2][tri]);

            simd_vec triAreaRcp(stackAreaRcp[tri]);

            tri_z_0 *= triAreaRcp;
            tri_z_1 *= triAreaRcp;
            tri_z_2 *= triAreaRcp;

            simd_veci tri_a0(stackA[0][tri]);
            simd_veci tri_a1(stackA[1][tri]);
            simd_veci tri_a2(stackA[2][tri]);

            simd_veci tri_b0(stackB[0][tri]);
            simd_veci tri_b1(stackB[1][tri]);
            simd_veci tri_b2(stackB[2][tri]);

            simd_veci tri_c0(stackC[0][tri]);
            simd_veci tri_c1(stackC[1][tri]);
            simd_veci tri_c2(stackC[2][tri]);

            simd_veci tri_a0_stride = tri_a0 << 1;
            simd_veci tri_a1_stride = tri_a1 << 1;
            simd_veci tri_a2_stride = tri_a2 << 1;

            int ac_min_x = minMaxXY[0][0][tri];
            int ac_min_y = minMaxXY[0][1][tri];
            int ac_max_x = minMaxXY[1][0][tri];
            int ac_max_y = minMaxXY[1][1][tri];

            simd_veci tri_fillX = simd_veci(ac_min_x) + pixelPatternX;

            simd_veci tri_b0_stride = tri_b0 << 1;
            simd_veci tri_b1_stride = tri_b1 << 1;
            simd_veci tri_b2_stride = tri_b2 << 1;

            simd_veci tri_fillY = simd_veci(ac_min_y) + pixelPatternY;

            simd_veci w0_row = (tri_a0 * tri_fillX) + (tri_b0 * tri_fillY) + tri_c0;
            simd_veci w1_row = (tri_a1 * tri_fillX) + (tri_b1 * tri_fillY) + tri_c1;
            simd_veci w2_row = (tri_a2 * tri_fillX) + (tri_b2 * tri_fillY) + tri_c2;

            U32 rowStoreStride = tile.tileSizeX * ac_min_y + 2 * ac_min_x;

            // Progress
            // Iterate y dimension
            for (U32 y = ac_min_y, yend = ac_max_y; y < yend; y += 2)
            {
                U32 storeStride = rowStoreStride;

                simd_veci w0 = w0_row;
                simd_veci w1 = w1_row;
                simd_veci w2 = w2_row;

                for (U32 x = ac_min_x, xend = ac_max_x; x < xend; x += 2)
                {
                    simd_veci coverageMask = w0 | w1 | w2;

                    if (coverageMask.IsEmpty())
                    {
                        continue;
                    }

                    simd_vec pixelDepth = simd_vec(w0) * tri_z_0 +
                                          simd_vec(w1) * tri_z_1 +
                                          simd_vec(w2) * tri_z_2;

                    F32 *depthLink = depthBuffer.GetDepthSequence(storeStride);

                    simd_vec prevDepth;
                    prevDepth.LoadAligned(depthLink);

                    simd_bool depthMask = (pixelDepth >= prevDepth) & simd_bool(coverageMask);

                    pixelDepth = SIMD_Select(depthMask, prevDepth, pixelDepth);
                    pixelDepth.StoreAligned(depthLink);

                    storeStride += 4;
                    w0 += tri_a0_stride;
                    w1 += tri_a1_stride;
                    w2 += tri_a2_stride;
                }

                rowStoreStride += 2 * tile.tileSizeX;
                w0_row += tri_b0_stride;
                w1_row += tri_b1_stride;
                w2_row += tri_b2_stride;
            }
        }
    }

    static void RasterizeTileData(  U32 tileShiftX, U32 tileShiftY,
                                    U32 tileSizeX, U32 tileSizeY )
    {
        SSE4_1SimdTraits<S32>::vec_type pixelPatternX, pixelPatternY;

        S32 StackAlign(64) patternX[4] = { 0, 1, 0, 1 };
        S32 StackAlign(64) patternY[4] = { 0, 0, 1, 1 };

        pixelPatternX.LoadAligned(patternX);
        pixelPatternY.LoadAligned(patternY);
    }



};

#endif