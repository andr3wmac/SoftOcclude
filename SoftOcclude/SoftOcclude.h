//--------------------------------------------------------------------------------------
// Copyright 2015 Andrew Mac
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

#include "Rasterizers/AABBoxRasterizerScalarST.h"
#include "Rasterizers/DepthBufferRasterizerScalarST.h"
#include "Rasterizers/TransformedModelScalar.h"

class SoftOcclusionTest
{
   protected:
      // Occluders
      DepthBufferRasterizer	 		*mpDBR;
	   DepthBufferRasterizerScalarST	*mpDBRScalarST;

      // Occludees
	   AABBoxRasterizer				   *mpAABB;
	   AABBoxRasterizerScalarST		*mpAABBScalarST;

   public:
      SoftOcclusionTest();
      ~SoftOcclusionTest();

      // Stats Functions
      void ComputeR2DBTime(UINT idx) { mpDBR->ComputeR2DBTime(idx); }
      UINT GetNumOccludersR2DB(UINT idx) { return mpDBR->GetNumOccludersR2DB(idx); }
      UINT GetNumRasterizedTriangles(UINT idx) { return mpDBR->GetNumRasterizedTriangles(idx); } 
      double GetRasterizeTime() { return mpDBR->GetRasterizeTime(); }
      UINT GetNumCulled(UINT idx) { return mpAABB->GetNumCulled(idx); } 
      UINT GetNumCulledTriangles(UINT idx) { return mpAABB->GetNumCulledTriangles(idx); }
      double GetDepthTestTime() { return mpAABB->GetDepthTestTime(); }
      UINT GetNumFCullCount() { return mpAABB->GetNumFCullCount(); }
      UINT GetNumTrisRendered() { return mpAABB->GetNumTrisRendered(); }

      // Shared Functions
      void SetViewProj(float4x4 *viewMatrix, float4x4 *projMatrix, UINT idx);
      void SetEnableFrustrumCulling(bool value);
      void ResetInsideFrustum();
      void SetCPURenderTargetPixels(UINT *pRenderTargetPixels, UINT idx);
      void Render(SoftFrustum *pFrustum, float pFov, UINT idx);

      // Occluder (mpDBR) Functions
      TransformedModelScalar* AddOccluder() { return mpDBR->AddOccluder(); }
      void RefreshOccluders() { mpDBR->RefreshOccluders(); }
      void SetOccluderSizeThreshold(float occluderSizeThreshold){ mpDBR->SetOccluderSizeThreshold(occluderSizeThreshold); }
      UINT GetNumOccluders() { return mpDBR->GetNumOccluders(); }
      UINT GetNumOccluderTris() { return mpDBR->GetNumTriangles(); }
      
      // Occludee (mpAABB) Functions
      //void CreateTransformedAABBoxes(CPUTAssetSet **pAssetSet, UINT numAssetSets) { mpAABB->CreateTransformedAABBoxes(pAssetSet, numAssetSets); }
      TransformedAABBoxScalar* AddOccludee() { return mpAABB->AddOccludee(); }
      void RefreshOccludees() { mpDBR->RefreshOccluders(); }
      void SetOccludeeSizeThreshold(float occludeeSizeThreshold){ mpAABB->SetOccludeeSizeThreshold(occludeeSizeThreshold); }
      void SetDepthTestTasks(UINT numTasks) { mpAABB->SetDepthTestTasks(numTasks); }
      UINT GetNumOccludees() { return mpAABB->GetNumOccludees(); }
      UINT GetNumOccludeeTris() { return mpDBR->GetNumTriangles(); }

      bool IsOccludeeVisible(UINT idx, UINT modelIdx) { return mpAABB->IsVisible(idx, modelIdx); }
};