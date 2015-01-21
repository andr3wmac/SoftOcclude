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
#include "TransformedModelScalar.h"

TransformedModelScalar::TransformedModelScalar()
   : mNumMeshes(0),
     mNumVertices(0),
     mNumTriangles(0)
{
   mInsideViewFrustum[0] = mInsideViewFrustum[1] = false;
   mTooSmall[0] = mTooSmall[1] = false;
   mpXformedPos[0] = mpXformedPos[1] = NULL;
}

TransformedModelScalar::~TransformedModelScalar()
{

}

void TransformedModelScalar::AddMesh(Vertex* pVertices, 
                            UINT pNumVertices, 
                            UINT* pIndices, 
                            UINT pNumIndices,
                            UINT pNumTriangles)
{
   mpMeshes[mNumMeshes].Initialize(pVertices, pNumVertices, pIndices, pNumIndices, pNumTriangles);
   mNumVertices += mpMeshes[mNumMeshes].GetNumVertices();
   mNumTriangles += mpMeshes[mNumMeshes].GetNumTriangles();
   mNumMeshes++;
}

void TransformedModelScalar::TooSmall(const BoxTestSetupScalar &setup, UINT idx)
{
   if(mInsideViewFrustum[idx])
   {
      mCumulativeMatrix[idx] = mWorldMatrix * setup.mViewProjViewport;
      
      float w = mBBCenterOS.x * mCumulativeMatrix[idx].r0.w +
              mBBCenterOS.y * mCumulativeMatrix[idx].r1.w +
              mBBCenterOS.z * mCumulativeMatrix[idx].r2.w +
              mCumulativeMatrix[idx].r3.w; 
   
      if(w > 1.0f)
      {
         mTooSmall[idx] = mRadiusSq < w * setup.radiusThreshold; 
      }
      else
      {
         // BB center is behind the near clip plane, making screen-space radius meaningless.
         // Assume visible.  This should be a safe assumption, as the frustum test says the bbox is visible.
         mTooSmall[idx] = false;
      }
   }
}


//------------------------------------------------------------------
// Determine is the occluder model is inside view frustum
//------------------------------------------------------------------
void TransformedModelScalar::InsideViewFrustum(SoftFrustum* frustum,
                           const BoxTestSetupScalar &setup,
                                    UINT idx)
{
   // andrewmac: TODO: FIX FRUSTRUM CULLING
   mInsideViewFrustum[idx] = frustum->IsVisible(mBBCenterWS, mBBHalfWS);

   if(mInsideViewFrustum[idx])
   {
      mCumulativeMatrix[idx] = mWorldMatrix * setup.mViewProjViewport;
      
      float w = mBBCenterOS.x * mCumulativeMatrix[idx].r0.w +
              mBBCenterOS.y * mCumulativeMatrix[idx].r1.w +
              mBBCenterOS.z * mCumulativeMatrix[idx].r2.w +
              mCumulativeMatrix[idx].r3.w; 
   
      if(w > 1.0f)
      {
         mTooSmall[idx] = mRadiusSq < w * setup.radiusThreshold; 
      }
      else
      {
         // BB center is behind the near clip plane, making screen-space radius meaningless.
         // Assume visible.  This should be a safe assumption, as the frustum test says the bbox is visible.
         mTooSmall[idx] = false;
      }
   }
}

//---------------------------------------------------------------------------------------------------
// Determine if the occluder size is sufficiently large enough to occlude other object sin the scene
// If so transform the occluder to screen space so that it can be rasterized to the cPU depth buffer
//---------------------------------------------------------------------------------------------------
void TransformedModelScalar::TransformMeshes(UINT start, 
                                     UINT end,
                                      UINT idx)
{
   if(mInsideViewFrustum[idx] && !mTooSmall[idx])
   {
      UINT totalNumVertices = 0;
      for(UINT meshId = 0; meshId < mNumMeshes; meshId++)
      {      
         totalNumVertices +=  mpMeshes[meshId].GetNumVertices();
         if(totalNumVertices < start)
         {
            continue;
         }
         mpMeshes[meshId].TransformVertices(mCumulativeMatrix[idx], start, end, idx);
      }
   }
}

//------------------------------------------------------------------------------------
// If the occluder is sufficiently large enough to occlude other objects in the scene 
// bin the triangles that make up the occluder into tiles to speed up rateraization
// Single threaded version
//------------------------------------------------------------------------------------
void TransformedModelScalar::BinTransformedTrianglesST(UINT taskId,
                                          UINT modelId,
                                          UINT start,
                                          UINT end,
                                          UINT* pBin,
                                          USHORT* pBinModel,
                                          USHORT* pBinMesh,
                                          USHORT* pNumTrisInBin,
                                          UINT idx)
{
   if(mInsideViewFrustum[idx] && !mTooSmall[idx])
   {
      UINT totalNumTris = 0;
      for(UINT meshId = 0; meshId < mNumMeshes; meshId++)
      {
         totalNumTris += mpMeshes[meshId].GetNumTriangles();
         if(totalNumTris < start)
         {
            continue;
         }
         mpMeshes[meshId].BinTransformedTrianglesST(taskId, modelId, meshId, start, end, pBin, pBinModel, pBinMesh, pNumTrisInBin, idx);
      }
   }
}

//------------------------------------------------------------------------------------
// If the occluder is sufficiently large enough to occlude other objects in the scene 
// bin the triangles that make up the occluder into tiles to speed up rateraization
// Multi threaded version
//------------------------------------------------------------------------------------
void TransformedModelScalar::BinTransformedTrianglesMT(UINT taskId,
                                          UINT modelId,
                                          UINT start,
                                          UINT end,
                                          UINT* pBin,
                                          USHORT* pBinModel,
                                          USHORT* pBinMesh,
                                          USHORT* pNumTrisInBin,
                                          UINT idx)
{
   if(mInsideViewFrustum[idx] && !mTooSmall[idx])
   {
      UINT totalNumTris = 0;
      for(UINT meshId = 0; meshId < mNumMeshes; meshId++)
      {
         totalNumTris += mpMeshes[meshId].GetNumTriangles();
         if(totalNumTris < start)
         {
            continue;
         }
         mpMeshes[meshId].BinTransformedTrianglesMT(taskId, modelId, meshId, start, end, pBin, pBinModel, pBinMesh, pNumTrisInBin, idx);
      }
   }
}

void TransformedModelScalar::Gather(float* xformedPos,
                           UINT meshId, 
                           UINT triId, 
                           UINT idx)
{
   mpMeshes[meshId].GetOneTriangleData(xformedPos, triId, idx);
}