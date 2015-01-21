//-----------------------------------------------------------------------------
// Copyright (c) 2015 Andrew Mac
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "Rasterizers/AABBoxRasterizerScalarST.h"
#include "Rasterizers/DepthBufferRasterizerScalarST.h"
#include "Rasterizers/TransformedModelScalar.h"

class SoftOcclusionTest
{
   protected:
      // Occluders
      DepthBufferRasterizer            *mpDBR;
      DepthBufferRasterizerScalarST    *mpDBRScalarST;

      // Occludees
      AABBoxRasterizer                 *mpAABB;
      AABBoxRasterizerScalarST         *mpAABBScalarST;

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
      TransformedAABBoxScalar* AddOccludee() { return mpAABB->AddOccludee(); }
      void RefreshOccludees() { mpDBR->RefreshOccluders(); }
      void SetOccludeeSizeThreshold(float occludeeSizeThreshold){ mpAABB->SetOccludeeSizeThreshold(occludeeSizeThreshold); }
      void SetDepthTestTasks(UINT numTasks) { mpAABB->SetDepthTestTasks(numTasks); }
      UINT GetNumOccludees() { return mpAABB->GetNumOccludees(); }
      UINT GetNumOccludeeTris() { return mpDBR->GetNumTriangles(); }

      bool IsOccludeeVisible(UINT idx, UINT modelIdx) { return mpAABB->IsVisible(idx, modelIdx); }
};