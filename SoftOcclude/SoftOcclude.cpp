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