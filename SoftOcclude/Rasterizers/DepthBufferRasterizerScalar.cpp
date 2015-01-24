//--------------------------------------------------------------------------------------
// Copyright 2013 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.
//-------------------------------------------------------------------------------------
#include "DepthBufferRasterizerScalar.h"

DepthBufferRasterizerScalar::DepthBufferRasterizerScalar()
   : DepthBufferRasterizer(),
     mNumModels(0),
     mNumVertices(0),
     mNumTriangles(0),
     mOccluderSizeThreshold(0.0f),
     mTimeCounter(0),
     mEnableFCulling(true)  
{
   mpXformedPos[0] = mpXformedPos[1] = NULL;
   mpRenderTargetPixels[0] = NULL;
   mpRenderTargetPixels[1] = NULL;
   mNumRasterized[0] = mNumRasterized[1] = NULL;

   mpBin[0] = mpBin[1] = NULL;
   mpBinModel[0] = mpBinModel[1] = NULL;
   mpBinMesh[0] = mpBinMesh[1] = NULL;
   mpNumTrisInBin[0] = mpNumTrisInBin[1] = NULL;

   for(UINT i = 0; i < AVG_COUNTER; i++)
   {
      mRasterizeTime[i] = 0.0;
   }
}

DepthBufferRasterizerScalar::~DepthBufferRasterizerScalar()
{
   SAFE_DELETE_ARRAY(mpXformedPos[0]);
   SAFE_DELETE_ARRAY(mpXformedPos[1]);
}

// andrewmac:
SoftOccluderScalar* DepthBufferRasterizerScalar::AddOccluder()
{
   SoftOccluderScalar* result = &mpTransformedModels[mNumModels];
   mNumModels++;

   return result;
}
void DepthBufferRasterizerScalar::RefreshOccluders()
{
   // Refresh Vert and Triangle Counts
   mNumVertices = 0;
   mNumTriangles = 0;
   for(UINT i = 0; i < mNumModels; i++)
   {
     mpXformedPosOffset[i] = mpTransformedModels[i].GetNumVertices();

     mpStartV[i] = mNumVertices;
     mNumVertices += mpTransformedModels[i].GetNumVertices();
     mpStartT[i] = mNumTriangles;
     mNumTriangles += mpTransformedModels[i].GetNumTriangles();
   }

   // Refresh Transforms of all Models and Meshs.
   mpStartV[mNumModels] = mNumVertices;
   mpStartT[mNumModels] = mNumTriangles;

   //multiply by 4 for x, y, z, w
   mpXformedPos[0] =  new float[mNumVertices * 4];
   mpXformedPos[1] =  new float[mNumVertices * 4];
   for(UINT i = 0; i < mNumModels; i++)
   {
      mpTransformedModels[i].SetXformedPos((float4*)&mpXformedPos[0][mpStartV[i] * 4],
                                   (float4*)&mpXformedPos[1][mpStartV[i] * 4],
                                    mpStartV[i]);
   }
}


//--------------------------------------------------------------------
// Clear depth buffer for a tile
//--------------------------------------------------------------------
void DepthBufferRasterizerScalar::ClearDepthTile(int startX, int startY, int endX, int endY, UINT idx)
{
   assert(startX % 2 == 0 && startY % 2 == 0);
   assert(endX % 2 == 0 && endY % 2 == 0);

   float* pDepthBuffer = (float*)mpRenderTargetPixels[idx];
   int width = endX - startX;

   // Note we need to account for tiling pattern here
   for(int r = startY; r < endY; r++)
   {
      int rowIdx = r * SCREENW + startX;
      memset(&pDepthBuffer[rowIdx], 0, sizeof(float) * width);
   }
}

void DepthBufferRasterizerScalar::SetViewProj(float4x4 *viewMatrix, float4x4 *projMatrix, UINT idx)
{
   mpViewMatrix[idx] = *viewMatrix;
   mpProjMatrix[idx] = *projMatrix;
}