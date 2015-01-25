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
//--------------------------------------------------------------------------------------
#ifndef __CPUT_SAMPLESTARTDX11_H__
#define __CPUT_SAMPLESTARTDX11_H__

#include <d3dx9math.h>
#include <stdio.h>
#include "CPUT_DX11.h"
#include <D3D11.h>
#include <xnamath.h>
#include <time.h>
#include "CPUTSprite.h"

#include "SoftOcclude.h"

//-----------------------------------------------------------------------------
class MySample : public CPUT_DX11
{
private:
   SoftOcclusionTest       *mOcclusionTest;

   float                   mfElapsedTime;
   
   CPUTCameraController    *mpCameraController;
   CPUTSprite              *mpDebugSprite;

   CPUTAssetSet            *mpShadowCameraSet;
   CPUTRenderTargetDepth   *mpShadowRenderTarget;

   CPUTText                *mpFPSCounter;
   //CPUTCamera              mCameraCopy[2];
   UINT                    mNumModels;
   CPUTModelDX11*          mpModels[1000];

   CPUTText                *mpOccludersText;
   CPUTText                *mpNumOccludersText;
   CPUTText                *mpOccludersR2DBText;
   CPUTText                *mpOccluderTrisText;
   CPUTText                *mpOccluderRasterizedTrisText;
   CPUTText                *mpRasterizeTimeText;
   CPUTSlider              *mpOccluderSizeSlider;

   CPUTText                *mpOccludeesText;
   CPUTText                *mpNumOccludeesText;
   CPUTText                *mpCulledText;
   CPUTText                *mpVisibleText;
   CPUTText                *mpOccludeeTrisText;
   CPUTText                *mpCulledTrisText;
   CPUTText                *mpVisibleTrisText;
   CPUTText                *mpDepthTestTimeText;
   CPUTSlider              *mpOccludeeSizeSlider;

   CPUTText                *mpTotalCullTimeText;

   CPUTCheckbox            *mpCullingCheckBox;
   CPUTCheckbox            *mpFCullingCheckBox;
   CPUTCheckbox            *mpDBCheckBox;
   CPUTCheckbox            *mpBBCheckBox;
   CPUTCheckbox            *mpVsyncCheckBox;
   CPUTCheckbox            *mpPipelineCheckBox;

   CPUTText                *mpDrawCallsText;
   CPUTSlider              *mpDepthTestTaskSlider;

   CPUTAssetSet            *mpAssetSetDBR[OCCLUDER_SETS];
   CPUTAssetSet            *mpAssetSetAABB[OCCLUDEE_SETS];
   CPUTAssetSet            *mpAssetSetSky;

   CPUTMaterialDX11        *mpShowDepthBufMtrlScalar;
   CPUTMaterialDX11        *mpShowDepthBufMtrlSSE;
   CPUTMaterialDX11        *mpShowDepthBufMtrl;
    
   //char                    *mpCPUDepthBuf[2];
   char                    *mpGPUDepthBuf;
    
   ID3D11Texture2D         *mpCPURenderTargetScalar[2];
   ID3D11Texture2D         *mpCPURenderTargetSSE[2];
   ID3D11Texture2D         *mpCPURenderTarget;

   ID3D11ShaderResourceView      *mpCPUSRVScalar[2];
   ID3D11ShaderResourceView      *mpCPUSRVSSE[2];
   ID3D11ShaderResourceView      *mpCPUSRV[2];

   UINT              rowPitch;

   UINT              mNumOccluders;
   UINT              mNumOccludersR2DB;
   UINT              mNumOccluderTris;
   UINT              mNumOccluderRasterizedTris;
   double            mRasterizeTime;
   float             mOccluderSizeThreshold;
    
   UINT              mNumOccludees;
   UINT              mNumCulled;
   UINT              mNumVisible;
   UINT              mNumOccludeeTris;
   UINT              mNumOccludeeCulledTris;
   UINT              mNumOccludeeVisibleTris;
   double            mDepthTestTime;
   float             mOccludeeSizeThreshold;

   double            mTotalCullTime;

   bool              mEnableCulling;
   bool              mEnableFCulling;
   bool              mViewDepthBuffer;
   bool              mViewBoundingBox;
   bool              mPipeline;

   UINT              mNumDrawCalls;
   UINT              mNumDepthTestTasks;
   //UINT              mCurrIdx;
   //UINT              mPrevIdx;
   //bool              mFirstFrame;

public:
   MySample() :
      mpCameraController(NULL),
      mpDebugSprite(NULL),
      mpShadowCameraSet(NULL),
      mpShadowRenderTarget(NULL),
      mpFPSCounter(NULL),
      mpOccludersText(NULL),
      mpNumOccludersText(NULL),
      mpOccludersR2DBText(NULL),
      mpOccluderTrisText(NULL),
      mpOccluderRasterizedTrisText(NULL),
      mpRasterizeTimeText(NULL),
      mpOccluderSizeSlider(NULL),
      mpOccludeesText(NULL),
      mpNumOccludeesText(NULL),
      mpCulledText(NULL),
      mpVisibleText(NULL),
      mpOccludeeTrisText(NULL),
      mpCulledTrisText(NULL),
      mpVisibleTrisText(NULL),
      mpDepthTestTimeText(NULL),
      mpOccludeeSizeSlider(NULL),
      mpTotalCullTimeText(NULL),
      mpCullingCheckBox(NULL),
      mpFCullingCheckBox(NULL),
      mpDBCheckBox(NULL),
      mpBBCheckBox(NULL),
      mpVsyncCheckBox(NULL),
      mpPipelineCheckBox(NULL),
      mpDrawCallsText(NULL),
      mpDepthTestTaskSlider(NULL),
      mpGPUDepthBuf(NULL),
      mNumOccluders(0),
      mNumOccludersR2DB(0),
      mNumOccluderTris(0),
      mNumOccluderRasterizedTris(0),
      mRasterizeTime(0.0),
      mOccluderSizeThreshold(gOccluderSizeThreshold),
      mNumCulled(0),
      mNumVisible(0),
      mNumOccludeeTris(0),
      mNumOccludeeCulledTris(0),
      mNumOccludeeVisibleTris(0),
      mDepthTestTime(0.0),
      mOccludeeSizeThreshold(gOccludeeSizeThreshold),
      mTotalCullTime(0.0),
      mEnableCulling(true),
      mEnableFCulling(true),
      mViewDepthBuffer(false),
      mViewBoundingBox(false),
      mPipeline(false),
      mNumDrawCalls(0),
      mNumDepthTestTasks(gDepthTestTasks),
      //mCurrIdx(0),
      //mPrevIdx(1),
      //mFirstFrame(true),
      mNumModels(0)
   {
      mpCPURenderTargetScalar[0] = NULL;
      mpCPURenderTargetScalar[1] = NULL;

      mpCPURenderTargetSSE[0] = NULL;
      mpCPURenderTargetSSE[1] = NULL;

      mpCPURenderTarget = NULL;

      mpCPUSRVScalar[0] = mpCPUSRVScalar[1] = NULL;
      mpCPUSRVSSE[0]      = mpCPUSRVSSE[1]    = NULL;
      mpCPUSRV[0]          = mpCPUSRV[1]       = NULL;

      for(UINT i = 0; i < OCCLUDER_SETS; i++)
      {
         mpAssetSetDBR[i] = NULL;
      }

      for(UINT i = 0; i < OCCLUDEE_SETS; i++)
      {
         mpAssetSetAABB[i] = NULL;
      }

      mpAssetSetSky = NULL;
      //mpCPUDepthBuf[0] = mpCPUDepthBuf[1] = NULL;
      mpShowDepthBufMtrlScalar = mpShowDepthBufMtrlSSE = mpShowDepthBufMtrl = NULL;

      // andrewmac:
      mOcclusionTest = new SoftOcclusionTest;
    }

   virtual ~MySample()
   {
      // Note: these two are defined in the base.  We release them because we addref them.
      SAFE_RELEASE(mpCamera);
      SAFE_RELEASE(mpShadowCamera);

      //_aligned_free(mpCPUDepthBuf[0]);
      //_aligned_free(mpCPUDepthBuf[1]);
      _aligned_free(mpGPUDepthBuf);
      SAFE_RELEASE(mpCPURenderTargetScalar[0]);
      SAFE_RELEASE(mpCPURenderTargetScalar[1]);
      SAFE_RELEASE(mpCPURenderTargetSSE[0]);
      SAFE_RELEASE(mpCPURenderTargetSSE[1]);

      SAFE_RELEASE(mpCPUSRVScalar[0]);
      SAFE_RELEASE(mpCPUSRVScalar[1]);
      SAFE_RELEASE(mpCPUSRVSSE[0]);
      SAFE_RELEASE(mpCPUSRVSSE[1]);
        
      //SAFE_DELETE(mpDBR);
      //SAFE_DELETE(mpAABB);
      // andrewmac:
      SAFE_DELETE(mOcclusionTest);

      for(UINT i = 0; i < OCCLUDER_SETS; i++)
      {
         SAFE_RELEASE(mpAssetSetDBR[i]);
      }
      SAFE_RELEASE(mpAssetSetAABB[0]);
      SAFE_RELEASE(mpAssetSetAABB[1]);
      SAFE_RELEASE(mpAssetSetSky); 

      SAFE_DELETE( mpCameraController );
      SAFE_DELETE( mpDebugSprite);
      SAFE_RELEASE(mpShadowCameraSet);
      SAFE_DELETE( mpShadowRenderTarget );

      SAFE_RELEASE( mpShowDepthBufMtrlScalar);
      SAFE_RELEASE( mpShowDepthBufMtrlSSE);

      // Release the models
      //for(UINT i = 0; i < mNumModels; i++)
      //{
      //   mpModels[i]->Release();
      //}

      CPUTModel::ReleaseStaticResources();
   }

   void* operator new(size_t i)
   {
      return _mm_malloc(i, 16);
   }

   void operator delete(void* p)
   {
      _mm_free(p);
   }

   //UINT *mpCPURenderTargetPixels;

   virtual CPUTEventHandledCode HandleKeyboardEvent(CPUTKey key);
   virtual CPUTEventHandledCode HandleMouseEvent(int x, int y, int wheel, CPUTMouseState state);
   virtual void                 HandleCallbackEvent( CPUTEventID Event, CPUTControlID ControlID, CPUTControl *pControl );

   // andrewmac:
   void RenderVisibleModels(CPUTAssetSet **pAssetSet,
                                           CPUTRenderParametersDX &renderParams,
                                           UINT numAssetSets);
   void RenderModels(CPUTAssetSet **pAssetSet,
                                    CPUTRenderParametersDX &renderParams,
                                    UINT numAssetSets);

   virtual void Create();
   virtual void Render(double deltaSeconds);
   virtual void Update(double deltaSeconds);
   virtual void ResizeWindow(UINT width, UINT height);
   virtual void TaskCleanUp();
   virtual void UpdateGPUDepthBuf();

   // andrewmac:
   void CreateTransformedModels(CPUTAssetSet **mpAssetSet, UINT numAssetSets);
   void CreateTransformedMeshes(SoftOccluderScalar* pTransformedModel, CPUTModelDX11 *pModel);
   void CreateTransformedAABBoxes(CPUTAssetSet **pAssetSet, UINT numAssetSets);
   void CreateAABBVertexIndexList(SoftOccludeeScalar* pTransformedBox, CPUTModelDX11 *pModel);

   // define some controls1
   static const CPUTControlID ID_MAIN_PANEL = 10;
   static const CPUTControlID ID_SECONDARY_PANEL = 20;
   static const CPUTControlID ID_FULLSCREEN_BUTTON = 100;
   static const CPUTControlID ID_NEXTMODEL_BUTTON = 200;
   static const CPUTControlID ID_TEST_CONTROL = 1000;
   static const CPUTControlID ID_IGNORE_CONTROL_ID = -1;

   static const CPUTControlID ID_OCCLUDERS = 1200;
   static const CPUTControlID ID_NUM_OCCLUDERS = 1300;
   static const CPUTControlID ID_NUM_OCCLUDERS_RASTERIZED_TO_DB = 1400;
   static const CPUTControlID ID_NUM_OCCLUDER_TRIS = 1500;
   static const CPUTControlID ID_NUM_OCCLUDER_RASTERIZED_TRIS = 1600;
   static const CPUTControlID ID_RASTERIZE_TIME = 1700;
   static const CPUTControlID ID_OCCLUDER_SIZE = 1800;

   static const CPUTControlID ID_OCCLUDEES = 1900;
   static const CPUTControlID ID_NUM_OCCLUDEES = 2000;
   static const CPUTControlID ID_NUM_CULLED = 2100;
   static const CPUTControlID ID_NUM_VISIBLE = 2150;
   static const CPUTControlID ID_NUM_OCCLUDEE_TRIS = 2200;
   static const CPUTControlID ID_NUM_OCCLUDEE_CULLED_TRIS = 2300;
   static const CPUTControlID ID_NUM_OCCLUDEE_VISIBLE_TRIS = 2350;
   static const CPUTControlID ID_DEPTHTEST_TIME = 2400;
   static const CPUTControlID ID_OCCLUDEE_SIZE = 2500;

   static const CPUTControlID ID_TOTAL_CULL_TIME = 2600;

   static const CPUTControlID ID_ENABLE_CULLING = 2700;
   static const CPUTControlID ID_ENABLE_FCULLING = 2800;
   static const CPUTControlID ID_DEPTH_BUFFER_VISIBLE = 2900;
   static const CPUTControlID ID_BOUNDING_BOX_VISIBLE = 3000;
   static const CPUTControlID ID_NUM_DRAW_CALLS = 3200;
   static const CPUTControlID ID_DEPTH_TEST_TASKS = 3300;
   static const CPUTControlID ID_VSYNC_ON_OFF = 3400;
   static const CPUTControlID ID_PIPELINE = 3500;
};
#endif // __CPUT_SAMPLESTARTDX11_H__
