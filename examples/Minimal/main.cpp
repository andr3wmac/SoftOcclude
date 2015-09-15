#include "core/common/types.h"
#include "core/meshRasterizer.h"
#include "core/depthBuffer.h"
#include "math/util/memory/stackAlign.h"

#include "math/simd/sse41.h"

#include "core/common/Bitmap.h"

#define SSE41_RASTERIZER MeshRasterizer< SSE4_1SimdTraits<F32>, SSE4_1SimdTraits<S32> > 


void SaveDepthBuffer(const char* filename, SimdDepthBuffer &depthBufferStruct)
{
    U32 pixels = depthBufferStruct.GetWidth() * depthBufferStruct.GetHeight();

    U8 *colorData = new U8[pixels * 3];

    F32* ddd = depthBufferStruct.GetDepthSequence(0);
    
    U32 idx = 0;

    //For each 2x2 tile row
    for (U32 h = 0; h < depthBufferStruct.GetHeight(); h+=2)
    {
        // for each 2x2 row cells
        for (U32 w = 0; w < depthBufferStruct.GetWidth(); w += 2)
        {
            for (U32 row = 0; row < 2; ++row)
            {
                for (U32 col = 0; col < 2; ++col)
                {
                    F32 depth = ddd[idx];

                    U8 color = depth > 0.1 ? 255 : 0;

                    U32 pos = ( (h + row) * depthBufferStruct.GetWidth() + (w + col) ) * 3;

                    colorData[pos] = color;
                    colorData[pos + 1] = color;
                    colorData[pos + 2] = color;

                    idx++;
                }
            }
        }
    }

    writeBMP(filename, colorData, depthBufferStruct.GetWidth(), depthBufferStruct.GetHeight());
}

int main()
{
    F32 StackAlign(64) tv1_x[4] = { 269.613068, 269.613068, 269.613068, 269.613068 };
    F32 StackAlign(64) tv1_y[4] = { 129.613052, 129.613052, 129.613052, 129.613052 };
    F32 StackAlign(64) tv1_z[4] = { 0.3, 0.4, 0.5, 0.6 };

    F32 StackAlign(64) tv2_x[4] = { 370.386932, 370.386932, 370.386932, 370.386932 };
    F32 StackAlign(64) tv2_y[4] = { 230.386948, 230.386948, 230.386948, 230.386948 };
    F32 StackAlign(64) tv2_z[4] = { 0.4, 0.5, 0.6, 0.7 };

    F32 StackAlign(64) tv3_x[4] = { 269.613068, 269.613068, 269.613068, 269.613068 };
    F32 StackAlign(64) tv3_y[4] = { 230.386948, 230.386948, 230.386948, 230.386948 };
    F32 StackAlign(64) tv3_z[4] = { 0.8, 0.9, 1.0, 1.1 };

    SSE41_RASTERIZER::SimdTriangleSet triangles(
        SSE41_RASTERIZER::SimdVertexSet(tv1_x, tv1_y, tv1_z),
        SSE41_RASTERIZER::SimdVertexSet(tv2_x, tv2_y, tv2_z),
        SSE41_RASTERIZER::SimdVertexSet(tv3_x, tv3_y, tv3_z)
        );
    

    SimdDepthBuffer depthBuffer(800, 600);
    depthBuffer.Clear();

    SSE4_1SimdTraits<S32>::vec_type pixelPatternX, pixelPatternY;

    S32 StackAlign(64) patternX[4] = { 0, 1, 0, 1 };
    S32 StackAlign(64) patternY[4] = { 0, 0, 1, 1 };

    pixelPatternX.LoadAligned(patternX);
    pixelPatternY.LoadAligned(patternY);

    SSE41_RASTERIZER::RasterizeToDepthBuffer(DepthBufferTile(   800,600,
                                                                0,800,
                                                                0,600),
                                             pixelPatternX,
                                             pixelPatternY,
                                             depthBuffer, triangles);

    SaveDepthBuffer("firstTriangle.bmp", depthBuffer);

    return 0;
}