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

#include "AABBoxRasterizerScalar.h"

AABBoxRasterizerScalar::AABBoxRasterizerScalar()
   : mNumModels(0),
     mOccludeeSizeThreshold(0.0f),
     mNumDepthTestTasks(0),
     mTimeCounter(0),
     mEnableFCulling(true)
{
   mpRenderTargetPixels[0] = NULL;
   mpRenderTargetPixels[1] = NULL;
   
   mNumCulled[0] = mNumCulled[1] =NULL;
   for(UINT i = 0; i < AVG_COUNTER; i++)
   {
      mDepthTestTime[i] = 0.0;
   }
}

AABBoxRasterizerScalar::~AABBoxRasterizerScalar()
{

}

// andrewmac:
TransformedAABBoxScalar* AABBoxRasterizerScalar::AddOccludee()
{
   TransformedAABBoxScalar* result = &mpTransformedAABBox[mNumModels];
   mNumModels++;

   return result;
}

void AABBoxRasterizerScalar::SetViewProjMatrix(float4x4 *viewMatrix, float4x4 *projMatrix, UINT idx)
{
   mViewMatrix[idx] = *viewMatrix;
   mProjMatrix[idx] = *projMatrix;
}