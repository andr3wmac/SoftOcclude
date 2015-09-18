#include "core/common/types.h"
#include "core/meshRasterizer.h"
#include "core/depthBuffer.h"
#include "math/util/memory/stackAlign.h"

#include "math/simd/sse41.h"
#include "math/simd/sse2.h"

#include "core/common/Bitmap.h"

#define SSE41_RASTERIZER MeshRasterizer< SSE4_1SimdTraits<F32>, SSE4_1SimdTraits<S32> > 
#define SSE2_RASTERIZER MeshRasterizer< SSE2SimdTraits<F32>, SSE2SimdTraits<S32> > 
#define SIMD_RASTERIZER SSE2_RASTERIZER

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
    

    SimdDepthBuffer depthBuffer(800, 600);
    depthBuffer.Clear();

    SIMD_RASTERIZER::RasterizeTileData(0, 0, 800, 600, depthBuffer);

    //SaveDepthBuffer("firstTriangle.bmp", depthBuffer);

    return 0;
}