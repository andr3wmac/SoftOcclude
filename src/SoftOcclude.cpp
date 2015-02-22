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
#include "common/Bitmap.h"

SoftOcclusionTest::SoftOcclusionTest()
{
   mCurrIdx = 0;

   mpDBRScalarST = new DepthBufferRasterizerScalarST;
   mpDBR = mpDBRScalarST;

   mpAABBScalarST = new AABBoxRasterizerScalarST;
   mpAABB = mpAABBScalarST;

   mpCPUDepthBuf[0] = NULL;
   mpCPUDepthBuf[1] = NULL;
}

SoftOcclusionTest::~SoftOcclusionTest()
{
   SAFE_DELETE(mpDBR);
   SAFE_DELETE(mpAABB);

   if ( mpCPUDepthBuf[0] )
      _aligned_free(mpCPUDepthBuf[0]);
   if ( mpCPUDepthBuf[1] )
      _aligned_free(mpCPUDepthBuf[1]);
}

void SoftOcclusionTest::Render(float4x4 *viewMatrix, float4x4 *projMatrix, SoftFrustum *pFrustum)
{
   // Flip/Flop Buffers
   // This is inherited from the Intel sample, I assume it's for threading.
   mCurrIdx = !mCurrIdx ? 1 : 0;

   // Set View/Projection Matrix
   mpDBR->SetViewProj(viewMatrix, projMatrix, mCurrIdx);
   mpAABB->SetViewProjMatrix(viewMatrix, projMatrix, mCurrIdx);

   // Set Render Targets
   mpDBR->SetCPURenderTargetPixels((UINT*)mpCPUDepthBuf[mCurrIdx], mCurrIdx);
   mpAABB->SetCPURenderTargetPixels((UINT*)mpCPUDepthBuf[mCurrIdx], mCurrIdx);

   // Perform Transformations and Rasterization
   mpDBR->TransformModelsAndRasterizeToDepthBuffer(pFrustum, pFrustum->mFov, mCurrIdx); 

   // Perform Depth Testing
   mpAABB->SetDepthSummaryBuffer(mpDBR->GetDepthSummaryBuffer(mCurrIdx), mCurrIdx);
   mpAABB->TransformAABBoxAndDepthTest(pFrustum, pFrustum->mFov, mCurrIdx);
}

void SoftOcclusionTest::SetScreenSize(int width, int height)
{
   mWidth = width;
   mHeight = height;

   // TODO: Destroy old buffer
   //       Store width/height to replace SCREENW and SCREENH constants
   mpCPUDepthBuf[0] = (char*)_aligned_malloc(sizeof(char) * width*height*4, 16);
   mpCPUDepthBuf[1] = (char*)_aligned_malloc(sizeof(char) * width*height*4, 16);
   
   // Clear buffers to zero.
   memset(mpCPUDepthBuf[0], 0, width * height *4);
   memset(mpCPUDepthBuf[1], 0, width * height *4);
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

void SoftOcclusionTest::SaveDepthBuffer(const char* filename)
{
   unsigned char depth;
   float *depthfloat;
   int tmpdepth;
   int maxdepth = 0;
   char* depthBuffer = GetDepthBuffer();
   unsigned char* outputRGB = new unsigned char[SCREENW * SCREENH * 3];

   for(int i = 0; i < SCREENW * SCREENH * 4; i += 4)
   {
      depthfloat = (float*)&depthBuffer[i];

      tmpdepth = (int)ceil(*depthfloat * 30000);
      maxdepth = tmpdepth > maxdepth ? tmpdepth : maxdepth;
   }

   float scale = 255.0f / maxdepth;

   int pos = 0;
   for(int i = 0; i < SCREENW * SCREENH * 4; i += 4)
   {
      depthfloat = (float*)&depthBuffer[i];

      tmpdepth = (int)ceil(*depthfloat * 20000);
      depth = (char)(tmpdepth * scale);
      depth = depth > 255 ? 255 : depth;

      outputRGB[pos] = depth;
      outputRGB[pos + 1] = depth;
      outputRGB[pos + 2] = depth;
      pos += 3;
   }

   writeBMP(filename, outputRGB, SCREENW, SCREENH);
}