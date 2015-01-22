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

      // Depth Buffer
      UINT                             mCurrIdx;
      char                             *mpCPUDepthBuf[2];

   public:
      SoftOcclusionTest();
      ~SoftOcclusionTest();

      // Configuration
      void SetScreenSize(int width, int height);
      void SetViewProj(float4x4 *viewMatrix, float4x4 *projMatrix);
      void SetEnableFrustrumCulling(bool value);
      void SetDepthTestTasks(UINT numTasks) { mpAABB->SetDepthTestTasks(numTasks); }
      void ResetInsideFrustum();

      // Occluder Management
      TransformedModelScalar* AddOccluder() { return mpDBR->AddOccluder(); }
      void RefreshOccluders() { mpDBR->RefreshOccluders(); }
      void SetOccluderSizeThreshold(float occluderSizeThreshold){ mpDBR->SetOccluderSizeThreshold(occluderSizeThreshold); }
      
      // Occludee Management
      TransformedAABBoxScalar* AddOccludee() { return mpAABB->AddOccludee(); }
      void SetOccludeeSizeThreshold(float occludeeSizeThreshold){ mpAABB->SetOccludeeSizeThreshold(occludeeSizeThreshold); }

      // Core Functionality
      char* GetDepthBuffer() { return mpCPUDepthBuf[mCurrIdx]; }
      void  Render(SoftFrustum *pFrustum, float pFov);
      bool  IsOccludeeVisible(UINT modelIdx) { return mpAABB->IsVisible(mCurrIdx, modelIdx); }

      // Library Statistics Functions
      UINT     GetNumOccluders()             { return mpDBR->GetNumOccluders(); }
      UINT     GetNumOccluderTris()          { return mpDBR->GetNumTriangles(); }
      UINT     GetNumOccludees()             { return mpAABB->GetNumOccludees(); }
      UINT     GetNumOccludeeTris()          { return mpDBR->GetNumTriangles(); }
      void     ComputeR2DBTime()             { mpDBR->ComputeR2DBTime(mCurrIdx); }
      UINT     GetNumOccludersR2DB()         { return mpDBR->GetNumOccludersR2DB(mCurrIdx); }
      UINT     GetNumRasterizedTriangles()   { return mpDBR->GetNumRasterizedTriangles(mCurrIdx); } 
      double   GetRasterizeTime()            { return mpDBR->GetRasterizeTime(); }
      UINT     GetNumCulled()                { return mpAABB->GetNumCulled(mCurrIdx); } 
      UINT     GetNumCulledTriangles()       { return mpAABB->GetNumCulledTriangles(mCurrIdx); }
      double   GetDepthTestTime()            { return mpAABB->GetDepthTestTime(); }
      UINT     GetNumFCullCount()            { return mpAABB->GetNumFCullCount(); }
      UINT     GetNumTrisRendered()          { return mpAABB->GetNumTrisRendered(); }
};