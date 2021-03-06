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
//
//--------------------------------------------------------------------------------------

#include "SoftOccluderMeshScalar.h"

SoftOccluderMeshScalar::SoftOccluderMeshScalar()
   : mNumVertices(0),
     mNumIndices(0),
     mNumTriangles(0),
     mpVertices(NULL),
     mpIndices(NULL)
{
   mpXformedPos[0] = mpXformedPos[1] = NULL;
}

SoftOccluderMeshScalar::~SoftOccluderMeshScalar()
{

}

void SoftOccluderMeshScalar::Initialize(
   Vertex* pVertices, UINT pNumVertices, 
   UINT* pIndices, UINT pNumIndices,
   UINT pNumTriangles)
{
   mNumVertices = pNumVertices;
   mNumIndices  = pNumIndices;
   mNumTriangles = pNumTriangles;
   mpVertices   = pVertices;
   mpIndices   = pIndices;
}

//-------------------------------------------------------------------
// Trasforms the occluder vertices to screen space once every frame
//-------------------------------------------------------------------
void SoftOccluderMeshScalar::TransformVertices(const float4x4 &cumulativeMatrix, 
                                   UINT start, 
                                   UINT end,
                                   UINT idx)
{
   for(UINT i = start; i <= end; i++)
   {
      float4 xform = TransformCoords(mpVertices[i].pos, cumulativeMatrix);
      float4 projected = xform / xform.w; 

      //set to all 0s if clipped by near clip plane
      mpXformedPos[idx][i] = xform.z <= xform.w ? projected : float4(0.0, 0.0, 0.0, 0.0);
   }
}

void SoftOccluderMeshScalar::Gather(float4 pOut[3], UINT triId, UINT idx)
{
   for(UINT i = 0; i < 3; i++)
   {
      UINT index = mpIndices[(triId * 3) + i];
      pOut[i] = mpXformedPos[idx][index];   
   }
}

//--------------------------------------------------------------------------------
// Bin the screen space transformed triangles into tiles. For single threaded version
//--------------------------------------------------------------------------------
void SoftOccluderMeshScalar::BinTransformedTrianglesST(UINT taskId,
                                         UINT modelId,
                                         UINT meshId,
                                         UINT start,
                                         UINT end,
                                         UINT* pBin,
                                         USHORT* pBinModel,
                                         USHORT* pBinMesh,
                                         USHORT* pNumTrisInBin,
                                         UINT idx)
{
   // working on one triangle at a time
   for(UINT index = start; index <= end; index++)
   {
      float4 xformedPos[3];
      Gather(xformedPos, index, idx);

      int fxPtX[3], fxPtY[3];
      for(int i = 0; i < 3; i++)
      {
         fxPtX[i] = (int)(xformedPos[i].x + 0.5);
         fxPtY[i] = (int)(xformedPos[i].y + 0.5);
      }
      
      // Compute triangle area
      int triArea = (fxPtX[1] - fxPtX[0]) * (fxPtY[2] - fxPtY[0]) - (fxPtX[0] - fxPtX[2]) * (fxPtY[0] - fxPtY[1]);
            
      // Find bounding box for screen space triangle in terms of pixels
      int startX = max(min(min(fxPtX[0], fxPtX[1]), fxPtX[2]), 0); 
      int endX   = min(max(max(fxPtX[0], fxPtX[1]), fxPtX[2]), mRasterData->mScreenWidth - 1);

      int startY = max(min(min(fxPtY[0], fxPtY[1]), fxPtY[2]), 0 );
      int endY   = min(max(max(fxPtY[0], fxPtY[1]), fxPtY[2]), mRasterData->mScreenHeight - 1);

      // Skip triangle if area is zero 
      if(triArea <= 0) continue;
      // Dont bin screen-clipped triangles
      if(endX < startX || endY < startY) continue;
         
      // Reject the triangle if any of its verts is behind the nearclip plane
      if(xformedPos[0].w > 0.0f && xformedPos[1].w > 0.0f && xformedPos[2].w > 0.0f) 
      {
         // Convert bounding box in terms of pixels to bounding box in terms of tiles
         int startXx = max(startX/mRasterData->mTileWidthInPixels, 0);
         int endXx   = min(endX/mRasterData->mTileWidthInPixels, mRasterData->mScreenWidthInTiles-1);
      
         int startYy = max(startY/mRasterData->mTileHeightInPixels, 0);
         int endYy   = min(endY/mRasterData->mTileHeightInPixels, mRasterData->mScreenHeightInTiles-1);

         // Add triangle to the tiles or bins that the bounding box covers
         int row, col;
         for(row = startYy; row <= endYy; row++)
         {
            int offset1 = mRasterData->mYOffset1_ST * row;
            int offset2 = mRasterData->mYOffset2_ST * row;
            for(col = startXx; col <= endXx; col++)
            {
               int idx1 = offset1 + (mRasterData->mXOffset1_ST * col) + taskId;
               int idx2 = offset2 + (mRasterData->mXOffset2_ST * col) + (taskId * MAX_TRIS_IN_BIN_ST) + pNumTrisInBin[idx1];
               pBin[idx2] = index;
               pBinModel[idx2] = modelId;
               pBinMesh[idx2] = meshId;
               pNumTrisInBin[idx1] += 1;
            }
         }
      }
   }
}

//--------------------------------------------------------------------------------
// Bin the screen space transformed triangles into tiles. For multi threaded version
//--------------------------------------------------------------------------------
void SoftOccluderMeshScalar::BinTransformedTrianglesMT(UINT taskId,
                                         UINT modelId,
                                         UINT meshId,
                                         UINT start,
                                         UINT end,
                                         UINT* pBin,
                                         USHORT* pBinModel,
                                         USHORT* pBinMesh,
                                         USHORT* pNumTrisInBin,
                                         UINT idx)
{
   // working on 4 triangles at a time
   for(UINT index = start; index <= end; index++)
   {
      float4 xformedPos[3];
      Gather(xformedPos, index, idx);
      
      int fxPtX[3], fxPtY[3];
      for(int i = 0; i < 3; i++)
      {
         fxPtX[i] = (int)(xformedPos[i].x + 0.5);
         fxPtY[i] = (int)(xformedPos[i].y + 0.5);
      }

      // Compute triangle area
      int triArea = (fxPtX[1] - fxPtX[0]) * (fxPtY[2] - fxPtY[0]) - (fxPtX[0] - fxPtX[2]) * (fxPtY[0] - fxPtY[1]);
      
      // Skip triangle if area is zero 
      if(triArea <= 0) continue;
      
      // Find bounding box for screen space triangle in terms of pixels
      int startX = max(min(min(fxPtX[0], fxPtX[1]), fxPtX[2]), 0);
     int endX   = min(max(max(fxPtX[0], fxPtX[1]), fxPtX[2]), mRasterData->mScreenWidth-1 );

     int startY = max(min(min(fxPtY[0], fxPtY[1]), fxPtY[2]), 0);
     int endY   = min(max(max(fxPtY[0], fxPtY[1]), fxPtY[2]), mRasterData->mScreenHeight-1);

      // Dont bin screen-clipped triangles
      if(endX < startX || endY < startY) continue;
            
      // Reject the triangle if any of its verts is behind the nearclip plane
      if(xformedPos[0].w > 0.0f && xformedPos[1].w > 0.0f && xformedPos[2].w > 0.0f) 
      {
         // Convert bounding box in terms of pixels to bounding box in terms of tiles
         int startXx = max(startX/mRasterData->mTileWidthInPixels, 0);
         int endXx   = min(endX/mRasterData->mTileWidthInPixels, mRasterData->mScreenWidthInTiles-1);

         int startYy = max(startY/mRasterData->mTileHeightInPixels, 0);
         int endYy   = min(endY/mRasterData->mTileHeightInPixels, mRasterData->mScreenHeightInTiles-1);

         // Add triangle to the tiles or bins that the bounding box covers
         int row, col;
         for(row = startYy; row <= endYy; row++)
         {
            int offset1 = mRasterData->mYOffset1_MT * row;
            int offset2 = mRasterData->mYOffset2_MT * row;
            for(col = startXx; col <= endXx; col++)
            {
               int idx1 = offset1 + (mRasterData->mXOffset1_MT * col) + (mRasterData->mTOffset1_MT * taskId);
               int idx2 = offset2 + (mRasterData->mXOffset2_MT * col) + (taskId * MAX_TRIS_IN_BIN_MT) + pNumTrisInBin[idx1];
               pBin[idx2] = index;
               pBinModel[idx2] = modelId;
               pBinMesh[idx2] = meshId;
               pNumTrisInBin[idx1] += 1;
            }
         }
      }
   }
}

void SoftOccluderMeshScalar::GetOneTriangleData(float* xformedPos, UINT triId, UINT idx)
{
   float4* pOut = (float4*) xformedPos;
   for(int i = 0; i < 3; i++)
   {
      UINT index = mpIndices[(triId * 3) + i];
      *(pOut + i) = mpXformedPos[idx][index];
   }
}