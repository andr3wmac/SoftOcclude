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

#include "SoftOcclude.h"

SoftOcclusionTest::SoftOcclusionTest()
{
   mpDBRScalarST = new DepthBufferRasterizerScalarST;
   mpDBR = mpDBRScalarST;

   mpAABBScalarST = new AABBoxRasterizerScalarST;
   mpAABB = mpAABBScalarST;
}

SoftOcclusionTest::~SoftOcclusionTest()
{
   SAFE_DELETE(mpDBR);
   SAFE_DELETE(mpAABB);
}

void SoftOcclusionTest::Render(SoftFrustum *pFrustum, float pFov, UINT idx)
{
   mpDBR->TransformModelsAndRasterizeToDepthBuffer(pFrustum, pFov, idx); 
   mpAABB->SetDepthSummaryBuffer(mpDBR->GetDepthSummaryBuffer(idx), idx);
   mpAABB->TransformAABBoxAndDepthTest(pFrustum, pFov, idx);
}

void SoftOcclusionTest::SetViewProj(float4x4 *viewMatrix, float4x4 *projMatrix, UINT idx)
{
   mpDBR->SetViewProj(viewMatrix, projMatrix, idx);
   mpAABB->SetViewProjMatrix(viewMatrix, projMatrix, idx);
}

void SoftOcclusionTest::SetEnableFrustrumCulling(bool value)
{
   mpDBR->SetEnableFCulling(value);
   mpAABB->SetEnableFCulling(value);
}
void SoftOcclusionTest::ResetInsideFrustum()
{
   mpDBR->ResetInsideFrustum();
   mpAABB->ResetInsideFrustum();
}
void SoftOcclusionTest::SetCPURenderTargetPixels(UINT *pRenderTargetPixels, UINT idx)
{
   mpDBR->SetCPURenderTargetPixels(pRenderTargetPixels, idx);
   mpAABB->SetCPURenderTargetPixels(pRenderTargetPixels, idx);
}