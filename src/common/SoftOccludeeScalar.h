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


#ifndef SOFTOCCLUDEESCALAR_H
#define SOFTOCCLUDEESCALAR_H

#include "Constants.h"
#include "../common/SoftFrustum.h"
#include "HelperScalar.h"

class SoftOccludeeScalar : public HelperScalar
{
   public:
      bool IsInsideViewFrustum(SoftFrustum* pFrustum);
      bool TransformAABBox(float4 xformedPos[], const float4x4 &cumulativeMatrix);
      bool RasterizeAndDepthTestAABBox(UINT *pRenderTargetPixels, const float4 pXformedPos[], UINT idx);

      bool IsTooSmall(const BoxTestSetupScalar &setup, float4x4 &cumulativeMatrix);

     void SetWorldMatrix(float4x4* worldMat) { mWorldMatrix = *worldMat; }
     void SetBoundsObjectSpace(float3 center, float3 half)
     {
        mBBCenter = center;
       mBBHalf = half;
        mRadiusSq = half.lengthSq();
     }
     void SetBoundsWorldSpace(float3 center, float3 half)
     {
       mBBCenterWS = center;
       mBBHalfWS = half;
     }        
      
   private:
      float4x4 mWorldMatrix;
         
      float3  mBBCenter;
      float3  mBBHalf;
      float   mRadiusSq;

      float3  mBBCenterWS;
      float3  mBBHalfWS;

      void Gather(float4 pOut[3], UINT triId, const float4 xformedPos[]);
};


#endif // TRANSFORMEDAABBOXSCALAR_H