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
#include "SoftwareOcclusionCulling.h"
#include "CPUTRenderTarget.h"
#include "CPUTTextureDX11.h"

const UINT SHADOW_WIDTH_HEIGHT = 256;

// set file to open
cString g_OpenFilePath;
cString g_OpenShaderPath;
cString g_OpenFileName;

extern float3 gLightDir;
extern char *gpDefaultShaderSource;

float gFarClipDistance = 2000.0f;

float gOccluderSizeThreshold = 1.5f;
float gOccludeeSizeThreshold = 0.01f;
UINT  gDepthTestTasks         = 20;

LARGE_INTEGER glFrequency; 

// Handle OnCreation events
//-----------------------------------------------------------------------------
void MySample::Create()
{    
   CPUTAssetLibrary *pAssetLibrary = CPUTAssetLibrary::GetAssetLibrary();

   gLightDir.normalize();

   // TODO: read from cmd line, using these as defaults
   //pAssetLibrary->SetMediaDirectoryName(    _L("Media"));

   CPUTGuiControllerDX11 *pGUI = CPUTGetGuiController();

   // create some controls
   CPUTButton     *pButton = NULL;
   pGUI->CreateButton(_L("Fullscreen"), ID_FULLSCREEN_BUTTON, ID_MAIN_PANEL, &pButton);
   
   wchar_t string[CPUT_MAX_STRING_LENGTH];
   pGUI->CreateText(    _L("Occluders                                              \t"), ID_OCCLUDERS, ID_MAIN_PANEL, &mpOccludersText);
    
   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tNumber of Models: \t%d"), mNumOccluders);
   pGUI->CreateText(string, ID_NUM_OCCLUDERS, ID_MAIN_PANEL, &mpNumOccludersText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tDepth rasterized models: %d"), mNumOccludersR2DB);
   pGUI->CreateText(string, ID_NUM_OCCLUDERS_RASTERIZED_TO_DB, ID_MAIN_PANEL, &mpOccludersR2DBText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tNumber of tris: \t\t%f"), mNumOccluderTris);
   pGUI->CreateText(string, ID_NUM_OCCLUDER_TRIS, ID_MAIN_PANEL, &mpOccluderTrisText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tDepth rasterized tris: \t%f"), mNumOccluderRasterizedTris);
   pGUI->CreateText(string, ID_NUM_OCCLUDER_RASTERIZED_TRIS, ID_MAIN_PANEL, &mpOccluderRasterizedTrisText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tDepth raterizer time: \t%f"), mRasterizeTime);
   pGUI->CreateText(string, ID_RASTERIZE_TIME, ID_MAIN_PANEL, &mpRasterizeTimeText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("Occluder Size Threshold: %0.4f"), mOccluderSizeThreshold);
   pGUI->CreateSlider(string, ID_OCCLUDER_SIZE, ID_MAIN_PANEL, &mpOccluderSizeSlider);
   mpOccluderSizeSlider->SetScale(0, 5.0, 51);
   mpOccluderSizeSlider->SetValue(mOccluderSizeThreshold);
   mpOccluderSizeSlider->SetTickDrawing(false);
    
   pGUI->CreateText(_L("Occludees                                              \t"), ID_OCCLUDEES, ID_MAIN_PANEL, &mpOccludeesText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tNumber of Models: \t%d"), mNumOccludees);
   pGUI->CreateText(string, ID_NUM_OCCLUDEES, ID_MAIN_PANEL, &mpNumOccludeesText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tModels culled: \t%d"), mNumCulled);
   pGUI->CreateText(string, ID_NUM_CULLED, ID_MAIN_PANEL, &mpCulledText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tModels visible: \t%d"), mNumVisible);
   pGUI->CreateText(string, ID_NUM_VISIBLE, ID_MAIN_PANEL, &mpVisibleText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tNumber of tris: \t%d"), (int)mNumOccludeeTris);
   pGUI->CreateText(string, ID_NUM_OCCLUDEE_TRIS, ID_MAIN_PANEL, &mpOccludeeTrisText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tCulled Triangles: \t%d"), (int)mNumOccludeeCulledTris);
   pGUI->CreateText(string, ID_NUM_OCCLUDEE_CULLED_TRIS, ID_MAIN_PANEL, &mpCulledTrisText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tVisible Triangles: \t%d"), (int)mNumOccludeeVisibleTris);
   pGUI->CreateText(string, ID_NUM_OCCLUDEE_VISIBLE_TRIS, ID_MAIN_PANEL, &mpVisibleTrisText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tVisible Triangles: \t%0.2f ms"), mDepthTestTime);
   pGUI->CreateText(string, ID_DEPTHTEST_TIME, ID_MAIN_PANEL, &mpDepthTestTimeText);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("Occludee Size Threshold: %0.4f"), mOccludeeSizeThreshold);
   pGUI->CreateSlider(string, ID_OCCLUDEE_SIZE, ID_MAIN_PANEL, &mpOccludeeSizeSlider);
   mpOccludeeSizeSlider->SetScale(0, 0.1f, 41);
   mpOccludeeSizeSlider->SetValue(mOccludeeSizeThreshold);
   mpOccludeeSizeSlider->SetTickDrawing(false);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("Total Cull time: %0.2f"), mTotalCullTime);
   pGUI->CreateText(string, ID_TOTAL_CULL_TIME, ID_MAIN_PANEL, &mpTotalCullTimeText);

   pGUI->CreateCheckbox(_L("Depth Test Culling"),  ID_ENABLE_CULLING, ID_MAIN_PANEL, &mpCullingCheckBox);
   pGUI->CreateCheckbox(_L("Frustum Culling"),  ID_ENABLE_FCULLING, ID_MAIN_PANEL, &mpFCullingCheckBox);
   pGUI->CreateCheckbox(_L("View Depth Buffer"),  ID_DEPTH_BUFFER_VISIBLE, ID_MAIN_PANEL, &mpDBCheckBox);
   pGUI->CreateCheckbox(_L("View Bounding Box"),  ID_BOUNDING_BOX_VISIBLE, ID_MAIN_PANEL, &mpBBCheckBox);
   pGUI->CreateCheckbox(_L("Vsync"), ID_VSYNC_ON_OFF, ID_MAIN_PANEL, &mpVsyncCheckBox);
   pGUI->CreateCheckbox(_L("Pipeline"), ID_PIPELINE, ID_MAIN_PANEL, &mpPipelineCheckBox);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("Number of draw calls: \t%d"), mNumDrawCalls);
   pGUI->CreateText(string, ID_NUM_DRAW_CALLS, ID_MAIN_PANEL, &mpDrawCallsText),
    
   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("Depth Test Tasks: \t\t%d"), mNumDepthTestTasks);
   pGUI->CreateSlider(string, ID_DEPTH_TEST_TASKS, ID_MAIN_PANEL, &mpDepthTestTaskSlider);
   mpDepthTestTaskSlider->SetScale(1, (float)NUM_DT_TASKS, 11);
   mpDepthTestTaskSlider->SetValue((float)mNumDepthTestTasks);
   mpDepthTestTaskSlider->SetTickDrawing(false);
   //mpAABB->SetDepthTestTasks(mNumDepthTestTasks);

   // andrewmac: 
   mOcclusionTest->SetScreenSize(SCREENW, SCREENH);
   mOcclusionTest->SetDepthTestTasks(mNumDepthTestTasks);

   //
   // Create Static text
   //
   pGUI->CreateText( _L("F1 for Help"), ID_IGNORE_CONTROL_ID, ID_SECONDARY_PANEL);
   pGUI->CreateText( _L("[Escape] to quit application"), ID_IGNORE_CONTROL_ID, ID_SECONDARY_PANEL);
   pGUI->CreateText( _L("A,S,D,F - move camera position"), ID_IGNORE_CONTROL_ID, ID_SECONDARY_PANEL);
   pGUI->CreateText( _L("Q - camera position up"), ID_IGNORE_CONTROL_ID, ID_SECONDARY_PANEL);
   pGUI->CreateText( _L("E - camera position down"), ID_IGNORE_CONTROL_ID, ID_SECONDARY_PANEL);
   pGUI->CreateText( _L("mouse + right click - camera look location"), ID_IGNORE_CONTROL_ID, ID_SECONDARY_PANEL);
   pGUI->CreateText( _L("size thresholds : computed using screen space metris"), ID_IGNORE_CONTROL_ID, ID_SECONDARY_PANEL);

   pGUI->SetActivePanel(ID_MAIN_PANEL);
   pGUI->DrawFPS(true);

   // Add our programatic (and global) material parameters
   CPUTMaterial::mGlobalProperties.AddValue( _L("cbPerFrameValues"), _L("$cbPerFrameValues") );
   CPUTMaterial::mGlobalProperties.AddValue( _L("cbPerModelValues"), _L("$cbPerModelValues") );
   CPUTMaterial::mGlobalProperties.AddValue( _L("_Shadow"), _L("$shadow_depth") );

   // Creating a render target to view the CPU rasterized depth buffer
   //mpCPUDepthBuf[0] = (char*)_aligned_malloc(sizeof(char) * SCREENW*SCREENH*4, 16);
   //mpCPUDepthBuf[1] = (char*)_aligned_malloc(sizeof(char) * SCREENW*SCREENH*4, 16);
   mpGPUDepthBuf    = (char*)_aligned_malloc(sizeof(char) * SCREENW*SCREENH*4, 16);

   CD3D11_TEXTURE2D_DESC cpuRenderTargetDescSSE
   (
      DXGI_FORMAT_R8G8B8A8_UNORM,
      SCREENW * 2, // TODO: round up to full tile sizes
      SCREENH / 2,
      1, // Array Size
      1, // MIP Levels
      D3D11_BIND_SHADER_RESOURCE,
      D3D11_USAGE_DEFAULT,
      0
   );

   HRESULT hr;
   hr = mpD3dDevice->CreateTexture2D(&cpuRenderTargetDescSSE, NULL, &mpCPURenderTargetSSE[0]);
   ASSERT(SUCCEEDED(hr), _L("Failed creating render target."));

   hr = mpD3dDevice->CreateTexture2D(&cpuRenderTargetDescSSE, NULL, &mpCPURenderTargetSSE[1]);
   ASSERT(SUCCEEDED(hr), _L("Failed creating render target."));

   hr = mpD3dDevice->CreateShaderResourceView(mpCPURenderTargetSSE[0], NULL, &mpCPUSRVSSE[0]);
   ASSERT(SUCCEEDED(hr), _L("Failed creating shader resource view."));

   hr = mpD3dDevice->CreateShaderResourceView(mpCPURenderTargetSSE[1], NULL, &mpCPUSRVSSE[1]);
   ASSERT(SUCCEEDED(hr), _L("Failed creating shader resource view."));

   // Corresponding texture object
   CPUTTextureDX11 *pDummyTex0 = new CPUTTextureDX11;
   pDummyTex0->SetTextureAndShaderResourceView(mpCPURenderTargetSSE[0], mpCPUSRVSSE[0]);
   pAssetLibrary->AddTexture( _L("$depthbuf_tex_SSE"), pDummyTex0 );
   SAFE_RELEASE(pDummyTex0);

   CPUTTextureDX11 *pDummyTex1 = new CPUTTextureDX11;
   pDummyTex1->SetTextureAndShaderResourceView(mpCPURenderTargetSSE[1], mpCPUSRVSSE[1]);
   pAssetLibrary->AddTexture( _L("$depthbuf_tex_SSE"), pDummyTex1 );
   SAFE_RELEASE(pDummyTex1);

   CD3D11_TEXTURE2D_DESC cpuRenderTargetDescScalar
   (
      DXGI_FORMAT_R8G8B8A8_UNORM,
      SCREENW, // TODO: round up to full tile sizes
      SCREENH,
      1, // Array Size
      1, // MIP Levels
      D3D11_BIND_SHADER_RESOURCE,
      D3D11_USAGE_DEFAULT,
      0
   );

   hr = mpD3dDevice->CreateTexture2D(&cpuRenderTargetDescScalar, NULL, &mpCPURenderTargetScalar[0]);
   ASSERT(SUCCEEDED(hr), _L("Failed creating render target."));

   hr = mpD3dDevice->CreateTexture2D(&cpuRenderTargetDescScalar, NULL, &mpCPURenderTargetScalar[1]);
   ASSERT(SUCCEEDED(hr), _L("Failed creating render target."));

   hr = mpD3dDevice->CreateShaderResourceView(mpCPURenderTargetScalar[0], NULL, &mpCPUSRVScalar[0]);
   ASSERT(SUCCEEDED(hr), _L("Failed creating shader resource view."));

   hr = mpD3dDevice->CreateShaderResourceView(mpCPURenderTargetScalar[1], NULL, &mpCPUSRVScalar[1]);
   ASSERT(SUCCEEDED(hr), _L("Failed creating shader resource view."));

   // Corresponding texture object
   CPUTTextureDX11 *pDummyTex2 = new CPUTTextureDX11;
   pDummyTex2->SetTextureAndShaderResourceView(mpCPURenderTargetScalar[0], mpCPUSRVScalar[0]);
   pAssetLibrary->AddTexture( _L("$depthbuf_tex_Scalar"), pDummyTex2 );
   SAFE_RELEASE(pDummyTex2);

   CPUTTextureDX11 *pDummyTex3 = new CPUTTextureDX11;
   pDummyTex3->SetTextureAndShaderResourceView(mpCPURenderTargetScalar[1], mpCPUSRVScalar[1]);
   pAssetLibrary->AddTexture( _L("$depthbuf_tex_Scalar"), pDummyTex3 );
   SAFE_RELEASE(pDummyTex3);

   // Create default shaders
   CPUTPixelShaderDX11  *pPS       = CPUTPixelShaderDX11::CreatePixelShaderFromMemory(            _L("$DefaultShader"), CPUT_DX11::mpD3dDevice,          _L("PSMain"), _L("ps_4_0"), gpDefaultShaderSource );
   CPUTPixelShaderDX11  *pPSNoTex  = CPUTPixelShaderDX11::CreatePixelShaderFromMemory(   _L("$DefaultShaderNoTexture"), CPUT_DX11::mpD3dDevice, _L("PSMainNoTexture"), _L("ps_4_0"), gpDefaultShaderSource );
   CPUTVertexShaderDX11 *pVS       = CPUTVertexShaderDX11::CreateVertexShaderFromMemory(          _L("$DefaultShader"), CPUT_DX11::mpD3dDevice,          _L("VSMain"), _L("vs_4_0"), gpDefaultShaderSource );
   CPUTVertexShaderDX11 *pVSNoTex  = CPUTVertexShaderDX11::CreateVertexShaderFromMemory( _L("$DefaultShaderNoTexture"), CPUT_DX11::mpD3dDevice, _L("VSMainNoTexture"), _L("vs_4_0"), gpDefaultShaderSource );

   // We just want to create them, which adds them to the library.  We don't need them any more so release them, leaving refCount at 1 (only library owns a ref)
   SAFE_RELEASE(pPS);
   SAFE_RELEASE(pPSNoTex);
   SAFE_RELEASE(pVS);
   SAFE_RELEASE(pVSNoTex);

   // load shadow casting material+sprite object
   cString ExecutableDirectory;
   CPUTOSServices::GetOSServices()->GetExecutableDirectory(&ExecutableDirectory);
   pAssetLibrary->SetMediaDirectoryName(  ExecutableDirectory+_L("..\\..\\Media\\"));

   mpShadowRenderTarget = new CPUTRenderTargetDepth();
   mpShadowRenderTarget->CreateRenderTarget( cString(_L("$shadow_depth")), SHADOW_WIDTH_HEIGHT, SHADOW_WIDTH_HEIGHT, DXGI_FORMAT_D32_FLOAT );

   mpDebugSprite = new CPUTSprite();
   mpDebugSprite->CreateSprite( -1.0f, -1.0f, 0.5f, 0.5f, _L("Sprite") );

   int width, height;
   CPUTOSServices::GetOSServices()->GetClientDimensions(&width, &height);

   // Depth buffer visualization material
   mpShowDepthBufMtrlScalar = (CPUTMaterialDX11*)CPUTAssetLibraryDX11::GetAssetLibrary()->GetMaterial( _L("showDepthBufScalar"));
   mpShowDepthBufMtrlSSE = (CPUTMaterialDX11*)CPUTAssetLibraryDX11::GetAssetLibrary()->GetMaterial( _L("showDepthBufSSE"));
        
   mpCPURenderTarget    = mpCPURenderTargetScalar[0];
   //mpCPURenderTarget[1] = mpCPURenderTargetScalar[1];
   mpCPUSRV[0]          = mpCPUSRVScalar[0];
   mpCPUSRV[1]          = mpCPUSRVScalar[1];
   mpShowDepthBufMtrl   = mpShowDepthBufMtrlScalar;
   rowPitch             = SCREENW * 4;

   // Call ResizeWindow() because it creates some resources that our blur material needs (e.g., the back buffer)
   ResizeWindow(width, height);

   CPUTRenderStateBlockDX11 *pBlock = new CPUTRenderStateBlockDX11();
   CPUTRenderStateDX11 *pStates = pBlock->GetState();

   // Override default sampler desc for our default shadowing sampler
   pStates->SamplerDesc[1].Filter         = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
   pStates->SamplerDesc[1].AddressU       = D3D11_TEXTURE_ADDRESS_BORDER;
   pStates->SamplerDesc[1].AddressV       = D3D11_TEXTURE_ADDRESS_BORDER;
   pStates->SamplerDesc[1].ComparisonFunc = D3D11_COMPARISON_GREATER;
   pBlock->CreateNativeResources();
   CPUTAssetLibrary::GetAssetLibrary()->AddRenderStateBlock( _L("$DefaultRenderStates"), pBlock );
   pBlock->Release(); // We're done with it.  The library owns it now.

   //
   // Load .set files to load the castle scene
   //
   pAssetLibrary->SetMediaDirectoryName(_L("..\\..\\Media\\Castle\\"));

   #ifdef DEBUG
      mpAssetSetDBR[0] = pAssetLibrary->GetAssetSet(_L("castleLargeOccluders"));
      ASSERT(mpAssetSetDBR[0], _L("Failed loading castle."));

      mpAssetSetDBR[1] = pAssetLibrary->GetAssetSet(_L("groundDebug"));
      ASSERT(mpAssetSetDBR[1], _L("Failed loading ground."));

      mpAssetSetAABB[0] = pAssetLibrary->GetAssetSet(_L("marketStallsDebug"));
      ASSERT(mpAssetSetAABB, _L("Failed loading marketStalls"));

      mpAssetSetAABB[1] = pAssetLibrary->GetAssetSet(_L("castleSmallDecorationsDebug"));
      ASSERT(mpAssetSetAABB, _L("Failed loading castleSmallDecorations"));
   #else
      mpAssetSetDBR[0] = pAssetLibrary->GetAssetSet(_L("castleLargeOccluders"));
      ASSERT(mpAssetSetDBR[0], _L("Failed loading castle."));

      mpAssetSetDBR[1] = pAssetLibrary->GetAssetSet(_L("ground"));
      ASSERT(mpAssetSetDBR[1], _L("Failed loading ground."));

      mpAssetSetAABB[0] = pAssetLibrary->GetAssetSet(_L("marketStalls"));
      ASSERT(mpAssetSetAABB, _L("Failed loading marketStalls"));

      mpAssetSetAABB[1] = pAssetLibrary->GetAssetSet(_L("castleSmallDecorations"));
      ASSERT(mpAssetSetAABB, _L("Failed loading castleSmallDecorations"));
   #endif

   mpAssetSetSky = pAssetLibrary->GetAssetSet(_L("sky"));
   ASSERT(mpAssetSetSky, _L("Failed loading sky"));

   // For every occluder model in the sene create a place holder 
   // for the CPU transformed vertices of the model.   
   //mpDBR->CreateTransformedModels(mpAssetSetDBR, OCCLUDER_SETS);
   // Get number of occluders in the scene
   //mNumOccluders = mpDBR->GetNumOccluders();
   // Get number of occluder triangles in the scene 
   //mNumOccluderTris = mpDBR->GetNumTriangles();

   // andrewmac
   CreateTransformedModels(mpAssetSetDBR, OCCLUDER_SETS);
   mNumOccluders = mOcclusionTest->GetNumOccluders();
   mNumOccluderTris = mOcclusionTest->GetNumOccluderTris();

   // For every occludee model in the scene create a place holder
   // for the triangles that make up the model axis aligned bounding box
   mpAssetSetAABB[2] = mpAssetSetDBR[0];
   mpAssetSetAABB[3] = mpAssetSetDBR[1];

   //mpAABB->CreateTransformedAABBoxes(mpAssetSetAABB, OCCLUDEE_SETS);
   // Get number of occludees in the scene
   //mNumOccludees = mpAABB->GetNumOccludees();
   // Get number of occluddee triangles in the scene
   //mNumOccludeeTris = mpAABB->GetNumTriangles();

   // andrewmac:
   //mOcclusionTest->CreateTransformedAABBoxes(mpAssetSetAABB, OCCLUDEE_SETS);
   CreateTransformedAABBoxes(mpAssetSetAABB, OCCLUDEE_SETS);
   mNumOccludees = mOcclusionTest->GetNumOccludees();
   mNumOccludeeTris = mOcclusionTest->GetNumOccludeeTris();
    
   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tNumber of Models: \t%d"), mNumOccluders);
   mpNumOccludersText->SetText(string);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tNumber of tris: \t\t%d"), mNumOccluderTris);
   mpOccluderTrisText->SetText(string);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tNumber of Model: \t%d"), mNumOccludees);
   mpNumOccludeesText->SetText(string);

   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tNumber of tris: \t\t%d"), mNumOccludeeTris);
   mpOccludeeTrisText->SetText(string);

   CPUTCheckboxState state;
   if(mEnableCulling)
   {
      state = CPUT_CHECKBOX_CHECKED;
   }
   else 
   {
      state = CPUT_CHECKBOX_UNCHECKED;
   }
   mpCullingCheckBox->SetCheckboxState(state);

   if(mEnableFCulling)
   {
      state = CPUT_CHECKBOX_CHECKED;
   }
   else 
   {
      state = CPUT_CHECKBOX_UNCHECKED;
   }
   mpFCullingCheckBox->SetCheckboxState(state);
   //mpDBR->SetEnableFCulling(mEnableFCulling);
   //mpAABB->SetEnableFCulling(mEnableFCulling);

   // andrewmac:
   mOcclusionTest->SetEnableFrustrumCulling(mEnableCulling);

   if(mViewDepthBuffer)
   {
      state = CPUT_CHECKBOX_CHECKED;
   }
   else 
   {
      state = CPUT_CHECKBOX_UNCHECKED;
   }
   mpDBCheckBox->SetCheckboxState(state);

   if(mSyncInterval)
   {
      state = CPUT_CHECKBOX_CHECKED;
   }
   else
   {
      state = CPUT_CHECKBOX_UNCHECKED;
   }
   mpVsyncCheckBox->SetCheckboxState(state);

   if(mPipeline)
   {
      state = CPUT_CHECKBOX_CHECKED;
   }
   else
   {
      state = CPUT_CHECKBOX_UNCHECKED;
   }
   mpPipelineCheckBox->SetCheckboxState(state);

   // Setting occluder size threshold in DepthBufferRasterizer
   //mpDBR->SetOccluderSizeThreshold(mOccluderSizeThreshold);
   // Setting occludee size threshold in AABBoxRasterizer
   //mpAABB->SetOccludeeSizeThreshold(mOccludeeSizeThreshold);

   // andrewmac
   mOcclusionTest->SetOccluderSizeThreshold(mOccluderSizeThreshold);
   mOcclusionTest->SetOccludeeSizeThreshold(mOccludeeSizeThreshold);
    
      //
      // If no cameras were created from the model sets then create a default simple camera
      // and add it to the camera array.
      //
   if( mpAssetSetDBR[0] && mpAssetSetDBR[0]->GetCameraCount() )
   {
      mpCamera = mpAssetSetDBR[0]->GetFirstCamera();
      mpCamera->AddRef(); 
   } else
   {
      mpCamera = new CPUTCamera();
      CPUTAssetLibraryDX11::GetAssetLibrary()->AddCamera( _L("SampleStart Camera"), mpCamera );

      mpCamera->SetPosition( 0.0f, 0.0f, 5.0f );
      // Set the projection matrix for all of the cameras to match our window.
      // TODO: this should really be a viewport matrix.  Otherwise, all cameras will have the same FOV and aspect ratio, etc instead of just viewport dimensions.
      mpCamera->SetAspectRatio(((float)width)/((float)height));
   }

   mpCamera->SetFov(XMConvertToRadians(60.0f)); // TODO: Fix converter's FOV bug (Maya generates cameras for which fbx reports garbage for fov)
   mpCamera->SetFarPlaneDistance(gFarClipDistance);
   mpCamera->SetPosition(27.0f, 2.0f, 47.0f);
   mpCamera->LookAt(41.0f, 8.0f, -50.0f);
   mpCamera->Update();

   // Set up the shadow camera (a camera that sees what the light sees)
   float3 lookAtPoint(0.0f, 0.0f, 0.0f);
   float3 half(1.0f, 1.0f, 1.0f);
   if( mpAssetSetDBR[0] )
   {
      mpAssetSetDBR[0]->GetBoundingBox( &lookAtPoint, &half );
   }
   float length = half.length();

   mpShadowCamera = new CPUTCamera();
   mpShadowCamera->SetFov(XMConvertToRadians(45));
   mpShadowCamera->SetAspectRatio(1.0f);
   float fov = mpShadowCamera->GetFov();
   float tanHalfFov = tanf(fov * 0.5f);
   float cameraDistance = length/tanHalfFov;
   float nearDistance = cameraDistance * 0.1f;
   mpShadowCamera->SetNearPlaneDistance(nearDistance);
   mpShadowCamera->SetFarPlaneDistance(2.0f * cameraDistance);
   CPUTAssetLibraryDX11::GetAssetLibrary()->AddCamera( _L("ShadowCamera"), mpShadowCamera );
   float3 shadowCameraPosition = lookAtPoint - gLightDir * cameraDistance;
   mpShadowCamera->SetPosition( shadowCameraPosition );
   mpShadowCamera->LookAt( lookAtPoint.x, lookAtPoint.y, lookAtPoint.z );
   mpShadowCamera->Update();

   mpCameraController = new CPUTCameraControllerFPS();
   mpCameraController->SetCamera(mpCamera);
   mpCameraController->SetLookSpeed(0.004f);
   mpCameraController->SetMoveSpeed(2.5f);

   gLightDir = float3(-40.48f, -142.493f, -3.348f);
   gLightDir = gLightDir.normalize();

   QueryPerformanceFrequency(&glFrequency); 
}

//-----------------------------------------------------------------------------
void MySample::Update(double deltaSeconds)
{
   mpCameraController->Update((float)deltaSeconds);
}

// Handle keyboard events
//-----------------------------------------------------------------------------
CPUTEventHandledCode MySample::HandleKeyboardEvent(CPUTKey key)
{
   static bool panelToggle = false;
   CPUTEventHandledCode    handled = CPUT_EVENT_UNHANDLED;
   cString fileName;
   CPUTGuiControllerDX11*  pGUI = CPUTGetGuiController();

   switch(key)
   {
   case KEY_F1:
      panelToggle = !panelToggle;
      if(panelToggle)
      {
         pGUI->SetActivePanel(ID_SECONDARY_PANEL);
      }
      else
      {
         pGUI->SetActivePanel(ID_MAIN_PANEL);
      }
      handled = CPUT_EVENT_HANDLED;
      break;
   case KEY_L:
      {
         static int cameraObjectIndex = 0;
         CPUTRenderNode *pCameraList[] = { mpCamera, mpShadowCamera };
         cameraObjectIndex = (++cameraObjectIndex) % (sizeof(pCameraList)/sizeof(*pCameraList));
         CPUTRenderNode *pCamera = pCameraList[cameraObjectIndex];
         mpCameraController->SetCamera( pCamera );
      }
      handled = CPUT_EVENT_HANDLED;
      break;
   case KEY_ESCAPE:
      handled = CPUT_EVENT_HANDLED;
      Shutdown();
      break;

    case KEY_1:
        {
            CPUTToggleFullScreenMode();
            break;
        }
    case KEY_2:
        {
            mEnableCulling = !mEnableCulling;
            CPUTCheckboxState state;
            if(mEnableCulling)
            {
                state = CPUT_CHECKBOX_CHECKED;
            }
            else 
            {
                state = CPUT_CHECKBOX_UNCHECKED;
                //memset(mpCPUDepthBuf[mCurrIdx], 0, SCREENW * SCREENH *4);

                mpOccludersR2DBText->SetText(         _L("\tDepth rasterized models: 0"));
                mpOccluderRasterizedTrisText->SetText(_L("\tDepth rasterized tris: \t0"));
                mpRasterizeTimeText->SetText(         _L("\tDepth rasterizer time: \t0 ms"));

                mpCulledText->SetText(       _L("\tModels culled: \t\t0"));
                mpVisibleText->SetText(      _L("\tModels visible: \t\t0"));
                mpCulledTrisText->SetText(   _L("\tCulled tris: \t\t0"));
                mpVisibleTrisText->SetText(   _L("\tVisible tris: \t\t0"));
                mpDepthTestTimeText->SetText(_L("\tDepth test time: \t0 ms"));
            }
            mpCullingCheckBox->SetCheckboxState(state);
            break;
        }
    case KEY_3:
        {
            mEnableFCulling = !mEnableFCulling;
            CPUTCheckboxState state;
            if(mEnableFCulling)
            {
                state = CPUT_CHECKBOX_CHECKED;
            }
            else
            {
                state = CPUT_CHECKBOX_UNCHECKED;
                //mpDBR->ResetInsideFrustum();
                //mpAABB->ResetInsideFrustum();
            // andrewmac:
            mOcclusionTest->ResetInsideFrustum();
            }
            mpFCullingCheckBox->SetCheckboxState(state);
            break;            
        }
    case KEY_4:
        {
            mViewDepthBuffer = !mViewDepthBuffer;
            CPUTCheckboxState state;
            if(mViewDepthBuffer)
            {
                state = CPUT_CHECKBOX_CHECKED;
            }
            else 
            {
                state = CPUT_CHECKBOX_UNCHECKED;
            }
            mpDBCheckBox->SetCheckboxState(state);
            break;
        }
    case KEY_5:
        {
            mViewBoundingBox = !mViewBoundingBox;
            CPUTCheckboxState state;
            if(mViewBoundingBox)
            {
                state = CPUT_CHECKBOX_CHECKED;
            }
            else
            {
                state = CPUT_CHECKBOX_UNCHECKED;
            }
            mpBBCheckBox->SetCheckboxState(state);
            break;
        }
    case KEY_6:
        {
            TaskCleanUp();
            CPUTCheckboxState state;
            
            //SAFE_DELETE_ARRAY(mpDBR);
            //SAFE_DELETE_ARRAY(mpAABB);

         // andrewmac:
         SAFE_DELETE(mOcclusionTest);

            state = CPUT_CHECKBOX_UNCHECKED;
            mpPipelineCheckBox->SetVisibility(false);
            mpDepthTestTaskSlider->SetVisibility(false);

            //mpDBRScalarST = new DepthBufferRasterizerScalarST;
            //mpDBR = mpDBRScalarST;
    
            //mpAABBScalarST = new AABBoxRasterizerScalarST;
            //mpAABB = mpAABBScalarST;

            //mpDBR->CreateTransformedModels(mpAssetSetDBR, OCCLUDER_SETS);        
            //mpDBR->SetOccluderSizeThreshold(mOccluderSizeThreshold);
            //mpAABB->CreateTransformedAABBoxes(mpAssetSetAABB, OCCLUDEE_SETS);
            //mpAABB->SetOccludeeSizeThreshold(mOccludeeSizeThreshold);

         // andrewmac: 
         mOcclusionTest = new SoftOcclusionTest;
         CreateTransformedModels(mpAssetSetDBR, OCCLUDER_SETS);
            mOcclusionTest->SetOccluderSizeThreshold(mOccluderSizeThreshold);
            CreateTransformedAABBoxes(mpAssetSetAABB, OCCLUDEE_SETS);
            mOcclusionTest->SetOccludeeSizeThreshold(mOccludeeSizeThreshold);
            break;
        }
    case KEY_7:
        {
            if(mSyncInterval == 1)
            {
                mSyncInterval = 0;
            }
            else 
            {
                mSyncInterval = 1;
            }
            CPUTCheckboxState state;
            if(mSyncInterval == 1)
            {
                state = CPUT_CHECKBOX_CHECKED;
            }
            else
            {
                state = CPUT_CHECKBOX_UNCHECKED;
            }
            mpVsyncCheckBox->SetCheckboxState(state);
            break;
        }
    case KEY_8:
        {
            TaskCleanUp();
            mPipeline = !mPipeline;
            CPUTCheckboxState state;
            if(mPipeline)
            {
                state = CPUT_CHECKBOX_CHECKED;
            }
            else
            {
                state = CPUT_CHECKBOX_UNCHECKED;
            }
            mpPipelineCheckBox->SetCheckboxState(state);
            break;
        }
    }
    

    // pass it to the camera controller
    if(handled == CPUT_EVENT_UNHANDLED)
    {
        handled = mpCameraController->HandleKeyboardEvent(key);
    }
    return handled;
}


void MySample::TaskCleanUp()
{
    //if(mEnableCulling)
    //{
    //    mCurrIdx = 0;
    //    mPrevIdx = 1;
    //    mFirstFrame = true;
    //}
}

// Handle mouse events
//-----------------------------------------------------------------------------
CPUTEventHandledCode MySample::HandleMouseEvent(int x, int y, int wheel, CPUTMouseState state)
{
    if( mpCameraController )
    {
        return mpCameraController->HandleMouseEvent(x, y, wheel, state);
    }
    return CPUT_EVENT_UNHANDLED;
}

// Handle any control callback events
//-----------------------------------------------------------------------------
void MySample::HandleCallbackEvent( CPUTEventID Event, CPUTControlID ControlID, CPUTControl *pControl )
{
    UNREFERENCED_PARAMETER(Event);
    UNREFERENCED_PARAMETER(pControl);
    cString SelectedItem;
    
    switch(ControlID)
    {
    case ID_FULLSCREEN_BUTTON:
    {
        CPUTToggleFullScreenMode();
        break;
    }
    case ID_DEPTH_BUFFER_VISIBLE:
    {
        TaskCleanUp();
        CPUTCheckboxState state = mpDBCheckBox->GetCheckboxState();
        if(state == CPUT_CHECKBOX_CHECKED)
        {
            mViewDepthBuffer = true;
        }
        else 
        {
            mViewDepthBuffer = false;
        }
        break;
    }
    case ID_BOUNDING_BOX_VISIBLE:
    {
        CPUTCheckboxState state = mpBBCheckBox->GetCheckboxState();
        if(state == CPUT_CHECKBOX_CHECKED)
        {
            mViewBoundingBox = true;
        }
        else 
        {
            mViewBoundingBox = false;
        }
        break;
    }
    case ID_OCCLUDER_SIZE:
    {
        TaskCleanUp();
        float occluderSize;
        mpOccluderSizeSlider->GetValue(occluderSize);
        mOccluderSizeThreshold = occluderSize;

        wchar_t string[CPUT_MAX_STRING_LENGTH];
        swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("Occluder Size Threshold: %0.4f"), mOccluderSizeThreshold);
        mpOccluderSizeSlider->SetText(string);

        //mpDBR->SetOccluderSizeThreshold(mOccluderSizeThreshold);
        // andrewmac:
      mOcclusionTest->SetOccluderSizeThreshold(mOccluderSizeThreshold);
      break;
    }
    case ID_OCCLUDEE_SIZE:
    {
        TaskCleanUp();
        float occludeeSize;
        mpOccludeeSizeSlider->GetValue(occludeeSize);
        mOccludeeSizeThreshold = occludeeSize;

        wchar_t string[CPUT_MAX_STRING_LENGTH];
        swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("Occludee Size Threshold: %0.4f"), mOccludeeSizeThreshold);
        mpOccludeeSizeSlider->SetText(string);
        //mpAABB->SetOccludeeSizeThreshold(mOccludeeSizeThreshold);
      // andrewmac:
      mOcclusionTest->SetOccludeeSizeThreshold(mOccludeeSizeThreshold);
        break;
    }
    case ID_DEPTH_TEST_TASKS:
    {
        TaskCleanUp();
        float numDepthTestTasks;
        mpDepthTestTaskSlider->GetValue(numDepthTestTasks);
        mNumDepthTestTasks = (UINT)numDepthTestTasks;

        wchar_t string[CPUT_MAX_STRING_LENGTH];
        swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("Depth Test Task: \t\t%d"), mNumDepthTestTasks);
        mpDepthTestTaskSlider->SetText(string);
        //mpAABB->SetDepthTestTasks(mNumDepthTestTasks);
        // andrewmac: 
      mOcclusionTest->SetDepthTestTasks(mNumDepthTestTasks);
      break;
    }

    case ID_ENABLE_CULLING:
    {
        TaskCleanUp();
        CPUTCheckboxState state = mpCullingCheckBox->GetCheckboxState();
        if(state)
        {
            mEnableCulling = true;
            //mFirstFrame = true;
        }
        else
        {
            mEnableCulling = false;
            //mFirstFrame = false;
            //memset(mpCPUDepthBuf[mCurrIdx], 0, SCREENW * SCREENH *4);

            mpOccludersR2DBText->SetText(         _L("\tDepth rasterized models: 0"));
            mpOccluderRasterizedTrisText->SetText(_L("\tDepth rasterized tris: \t0"));
            mpRasterizeTimeText->SetText(         _L("\tDepth rasterizer time: \t0 ms"));

            mpCulledText->SetText(       _L("\tModels culled: \t\t0"));
            mpVisibleText->SetText(      _L("\tModels visible: \t\t0"));
            mpCulledTrisText->SetText(   _L("\tCulled tris: \t\t0"));
            mpVisibleTrisText->SetText(  _L("\tVisible tris: \t\t0"));
            mpDepthTestTimeText->SetText(_L("\tDepth test time: \t0 ms"));
            mpTotalCullTimeText->SetText(_L("\tTotal Cull time: \t0 ms"));
        }
        break;
    }
   case ID_ENABLE_FCULLING:
   {
      TaskCleanUp();
      CPUTCheckboxState state = mpFCullingCheckBox->GetCheckboxState();
      if(state)
      {
         mEnableFCulling = true;
      }
      else
      {
         mEnableFCulling = false;
         //mpDBR->ResetInsideFrustum();
         //mpAABB->ResetInsideFrustum();
         // andrewmac:
         mOcclusionTest->ResetInsideFrustum();
      }
      //mpDBR->SetEnableFCulling(mEnableFCulling);
      //mpAABB->SetEnableFCulling(mEnableFCulling);
      // andrewmac:
      mOcclusionTest->SetEnableFrustrumCulling(mEnableFCulling);
      break;
    }
    case ID_VSYNC_ON_OFF:
    {
        CPUTCheckboxState state = mpVsyncCheckBox->GetCheckboxState();
        if(state)
        {
            mSyncInterval = 1;
        }
        else
        {
            mSyncInterval = 0;
        }
        break;
    }
    case ID_PIPELINE:
    {
        TaskCleanUp();
        CPUTCheckboxState state = mpPipelineCheckBox->GetCheckboxState();
        if(state)
        {
            mPipeline = true;
            //mFirstFrame = true;
        }
        else
        {
            mPipeline = false;
        }
        break;
    }
    default:
        break;
    }
}

// Handle resize events
//-----------------------------------------------------------------------------
void MySample::ResizeWindow(UINT width, UINT height)
{
    CPUTAssetLibrary *pAssetLibrary = CPUTAssetLibrary::GetAssetLibrary();

    // Before we can resize the swap chain, we must release any references to it.
    // We could have a "AssetLibrary::ReleaseSwapChainResources(), or similar.  But,
    // Generic "release all" works, is simpler to implement/maintain, and is not performance critical.
    pAssetLibrary->ReleaseTexturesAndBuffers();

    // Resize the CPUT-provided render target
    CPUT_DX11::ResizeWindow( width, height );

    // Resize any application-specific render targets here
    if( mpCamera ) mpCamera->SetAspectRatio(((float)width)/((float)height));

    pAssetLibrary->RebindTexturesAndBuffers();
}

void MySample::UpdateGPUDepthBuf()
{
    unsigned char depth;
    float *depthfloat;
    int tmpdepth;
    int maxdepth = 0;

    // andrewmac
    char* depthBuffer = mOcclusionTest->GetDepthBuffer();

    for(int i = 0; i < SCREENW * SCREENH * 4; i += 4)
    {
        //depthfloat = (float*)&mpCPUDepthBuf[idx][i];
        // andrewmac:
        depthfloat = (float*)&depthBuffer[i];

        tmpdepth = (int)ceil(*depthfloat * 30000);
        maxdepth = tmpdepth > maxdepth ? tmpdepth : maxdepth;
    }

    float scale = 255.0f / maxdepth;

    for(int i = 0; i < SCREENW * SCREENH * 4; i += 4)
    {
        //depthfloat = (float*)&mpCPUDepthBuf[idx][i];
        // andrewmac:
        depthfloat = (float*)&depthBuffer[i];

        tmpdepth = (int)ceil(*depthfloat * 20000);
        depth = (char)(tmpdepth * scale);
        depth = depth > 255 ? 255 : depth;

        mpGPUDepthBuf[i + 0] = depth;
        mpGPUDepthBuf[i + 1] = depth;
        mpGPUDepthBuf[i + 2] = depth;
        mpGPUDepthBuf[i + 3] = depth;
    }
}

//------------------------------------------------------------------------
// Go through the list of models in the asset set and render only those 
// models that are marked as visible by the software occlusion culling test
//-------------------------------------------------------------------------
void MySample::RenderVisibleModels(CPUTAssetSet **pAssetSet,
                                           CPUTRenderParametersDX &renderParams,
                                           UINT numAssetSets)
{
    int count = 0;
    for(UINT modelId = 0; modelId < mNumModels; modelId++)
    {
        if(mOcclusionTest->IsOccludeeVisible(modelId))
        {
            mpModels[modelId]->Render(renderParams);
            count++;
        }
    }

    mNumCulled = mNumModels - count;
}


//------------------------------------------------------------------------
// Go through the list of models in the asset set and render only those 
// models that are marked as visible by the software occlusion culling test
//-------------------------------------------------------------------------
void MySample::RenderModels(CPUTAssetSet **pAssetSet,
                                    CPUTRenderParametersDX &renderParams,
                                    UINT numAssetSets)
{
   int count = 0;
   int triCount = 0;

   CPUTModelDX11::ResetFCullCount();

   BoxTestSetupScalar setup;
   setup.Init(*mpCamera->GetViewMatrix(), *mpCamera->GetProjectionMatrix(), viewportMatrix, mpCamera->GetFov(), mOccludeeSizeThreshold);

   float4x4 cumulativeMatrix;

   for(UINT assetId = 0, modelId = 0; assetId < numAssetSets; assetId++)
   {
      for(UINT nodeId = 0; nodeId< pAssetSet[assetId]->GetAssetCount(); nodeId++)
      {
         CPUTRenderNode* pRenderNode = NULL;
         CPUTResult result = pAssetSet[assetId]->GetAssetByIndex(nodeId, &pRenderNode);
         ASSERT((CPUT_SUCCESS == result), _L ("Failed getting asset by index")); 
         if(pRenderNode->IsModel())
         {
            //if(!mpTransformedAABBox[modelId].IsTooSmall(setup, cumulativeMatrix))
            //{
               CPUTModelDX11* model = (CPUTModelDX11*)pRenderNode;
               ASSERT((model != NULL), _L("model is NULL"));

               model = (CPUTModelDX11*)pRenderNode;
               model->Render(renderParams);
               count++;
               //triCount += mpNumTriangles[modelId];
            //}
            modelId++;            
         }
         pRenderNode->Release();
      }
   }
   //mNumFCullCount = CPUTModelDX11::GetFCullCount();
   mNumCulled = mNumModels - count;
   //mNumTrisRendered = triCount;
}

//#include "xnamath.h"
//static ID3D11UnorderedAccessView *gpNullUAVs[CPUT_MATERIAL_MAX_TEXTURE_SLOTS] = {0};
//-----------------------------------------------------------------------------
void MySample::Render(double deltaSeconds)
{
   CPUTRenderParametersDX renderParams(mpContext);

   // If mViewBoundingBox is enabled then draw the axis aligned bounding box 
   // for all the model in the scene. FYI This will affect frame rate.
   if(mViewBoundingBox)
   {
      renderParams.mShowBoundingBoxes = true;
   }
   else
   {
      renderParams.mShowBoundingBoxes = false;
   }    

   /*
   if(mEnableCulling) 
   {
      if(!mFirstFrame)
      {
         UINT tmpId = mCurrIdx;
         mCurrIdx = mPrevIdx;
         mPrevIdx = tmpId;
      }    
   }

   mCameraCopy[mCurrIdx] = *mpCamera;
   */

   // Clear back buffer
   const float clearColor[] = { 0.0993f, 0.0993f, 0.0993f, 1.0f };
   mpContext->ClearRenderTargetView( mpBackBufferRTV,  clearColor );
   mpContext->ClearDepthStencilView( mpDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, 0);

   // Set the camera transforms so that the occluders can be transformed 
   //mpDBR->SetViewProj(mpCamera->GetViewMatrix(), (float4x4*)mpCamera->GetProjectionMatrix(), mCurrIdx);

   // Set the camera transforms so that the occludee abix aligned bounding boxes (AABB) can be transformed
   //mpAABB->SetViewProjMatrix(mpCamera->GetViewMatrix(), (float4x4*)mpCamera->GetProjectionMatrix(), mCurrIdx);

   // andrewmac:
   mOcclusionTest->SetViewProj(mpCamera->GetViewMatrix(), (float4x4*)mpCamera->GetProjectionMatrix());

   // If view frustum culling is enabled then determine which occluders and occludees are 
   // inside the view frustum and run the software occlusion culling on only the those models
   if(mEnableFCulling)
   {
      renderParams.mRenderOnlyVisibleModels = true;
   }
   else
   {
      renderParams.mRenderOnlyVisibleModels = false;
   }

   // if software occlusion culling is enabled
   if(mEnableCulling)
   {
      // Set the Depth Buffer
      //mpCPURenderTargetPixels = (UINT*)mpCPUDepthBuf[mCurrIdx];
        
      //mpDBR->SetCPURenderTargetPixels(mpCPURenderTargetPixels, mCurrIdx);
      // Transform the occluder models and rasterize them to the depth buffer
      //mpDBR->TransformModelsAndRasterizeToDepthBuffer(&mCameraCopy[mCurrIdx], mCurrIdx);
    
        
      //mpAABB->SetCPURenderTargetPixels(mpCPURenderTargetPixels, mCurrIdx);
      //mpAABB->SetDepthSummaryBuffer(mpDBR->GetDepthSummaryBuffer(mCurrIdx), mCurrIdx);
      // Transform the occludee AABB, rasterize and depth test to determine is occludee is visible or occluded 
      //mpAABB->TransformAABBoxAndDepthTest(&mCameraCopy[mCurrIdx], mCurrIdx);    

      // andrewmac
      SoftFrustum viewFrustum;
      memcpy(viewFrustum.mpPosition, mpCamera->mFrustum.mpPosition, sizeof(viewFrustum.mpPosition));
      memcpy(viewFrustum.mpNormal, mpCamera->mFrustum.mpNormal, sizeof(viewFrustum.mpNormal));

      mOcclusionTest->Render(&viewFrustum, mpCamera->GetFov());
   }
    
   // If mViewDepthBuffer is enabled then blit the CPU rasterized depth buffer to the frame buffer
   if(mViewDepthBuffer)
   {
      mpShowDepthBufMtrl->SetRenderStates(renderParams);

      // Update the GPU-side depth buffer
      UpdateGPUDepthBuf();
      mpContext->UpdateSubresource(mpCPURenderTarget, 0, NULL, mpGPUDepthBuf, rowPitch, 0);            
      mpContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
      mpContext->Draw(3, 0);
   }
   // else render the (frustum culled) occluders and only the visible occludees
   else
   {
      CPUTMeshDX11::ResetDrawCallCount();

      if(mpAssetSetAABB) 
      {
         // if occlusion culling is enabled then render only the visible occludees in the scene
         if(mEnableCulling)
         {
            //renderParams.mpCamera = &mCameraCopy[mCurrIdx];
            //mpAABB->RenderVisible(mpAssetSetAABB, renderParams, OCCLUDEE_SETS, mCurrIdx); 
            // andrewmac:
            renderParams.mpCamera = mpCamera;
            RenderVisibleModels(mpAssetSetAABB, renderParams, OCCLUDEE_SETS);
         }
         // else render all the (25,000) occludee models in the scene
         else
         {
            //renderParams.mpCamera = &mCameraCopy[mCurrIdx];
            //mpAABB->Render(mpAssetSetAABB, renderParams, OCCLUDEE_SETS, mCurrIdx);
            // andrewmac:
            renderParams.mpCamera = mpCamera;
            RenderModels(mpAssetSetAABB, renderParams, OCCLUDEE_SETS);
         }

         // Render the sky?
         if(mpAssetSetSky) { mpAssetSetSky->RenderRecursive(renderParams); }
      }
      mNumDrawCalls = CPUTMeshDX11::GetDrawCallCount();
   }

   wchar_t string[CPUT_MAX_STRING_LENGTH];
   if(mEnableCulling)
   {
      //mpDBR->ComputeR2DBTime(mCurrIdx);
      //mNumOccludersR2DB = mpDBR->GetNumOccludersR2DB(mCurrIdx);
      //mNumOccluderRasterizedTris = mpDBR->GetNumRasterizedTriangles(mCurrIdx);
      //mNumCulled = mpAABB->GetNumCulled(mCurrIdx);
      //mNumOccludeeCulledTris = mpAABB->GetNumCulledTriangles(mCurrIdx);

      //mRasterizeTime = mpDBR->GetRasterizeTime();

      // andrewmac:
      mOcclusionTest->ComputeR2DBTime();
      mNumOccludersR2DB = mOcclusionTest->GetNumOccludersR2DB();
      mNumOccluderRasterizedTris = mOcclusionTest->GetNumRasterizedTriangles();
      //mNumCulled = mOcclusionTest->GetNumCulled();
      mNumOccludeeCulledTris = mOcclusionTest->GetNumCulledTriangles();
      mRasterizeTime = mOcclusionTest->GetRasterizeTime();
        
      mNumVisible = mNumOccludees - mNumCulled;
      mNumOccludeeVisibleTris = mNumOccludeeTris - mNumOccludeeCulledTris;
        
      //mDepthTestTime = mpAABB->GetDepthTestTime();
      // andrewmac:
      mDepthTestTime = mOcclusionTest->GetDepthTestTime();
      mTotalCullTime = mRasterizeTime + mDepthTestTime;
        
      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tDepth rasterized models: %d"), mNumOccludersR2DB);
      mpOccludersR2DBText->SetText(string);

      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tDepth rasterized tris: \t%d"), (int)mNumOccluderRasterizedTris);
      mpOccluderRasterizedTrisText->SetText(string);

      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tDepth rasterizer time: \t%0.2f ms"), mRasterizeTime * 1000.0f);
      mpRasterizeTimeText->SetText(string);

      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tModels culled: \t\t%d"), mNumCulled);
      mpCulledText->SetText(string);

      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tModels visible: \t\t%d"), mNumVisible);
      mpVisibleText->SetText(string);

      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tCulled tris: \t\t%d"), (int)mNumOccludeeCulledTris);
      mpCulledTrisText->SetText(string);

      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tVisible tris: \t\t%d"), (int)mNumOccludeeVisibleTris);
      mpVisibleTrisText->SetText(string);

      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tDepth test time: \t%0.2f ms"), mDepthTestTime * 1000.0f);
      mpDepthTestTimeText->SetText(string);        

      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tTotal Cull time: \t%0.2f ms"), mTotalCullTime * 1000.0f);
      mpTotalCullTimeText->SetText(string);
   }
   else
   {
      UINT fCullCount = 0;
      if(mEnableFCulling)
      {
         //fCullCount = mpAABB->GetNumFCullCount();
      // andrewmac:
      fCullCount = mOcclusionTest->GetNumFCullCount();
      }

      //mNumCulled = mpAABB->GetNumCulled(mCurrIdx) + fCullCount;
      //mNumOccludeeVisibleTris = mpAABB->GetNumTrisRendered();

      // andrewmac:
      mNumCulled = mOcclusionTest->GetNumCulled() + fCullCount;
      mNumOccludeeVisibleTris = mOcclusionTest->GetNumTrisRendered();

      mNumOccludeeCulledTris = mNumOccludeeTris - mNumOccludeeVisibleTris;
      mNumVisible = mNumOccludees - mNumCulled;
        
      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tModels culled: \t\t%d"), mNumCulled);
      mpCulledText->SetText(string);

      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tModels visible: \t\t%d"), mNumVisible);
      mpVisibleText->SetText(string);

      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tCulled tris: \t\t%d"), (int)mNumOccludeeCulledTris);
      mpCulledTrisText->SetText(string);

      swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("\tVisible tris: \t\t%d"), (int)mNumOccludeeVisibleTris);
      mpVisibleTrisText->SetText(string);
   }
    
   swprintf_s(&string[0], CPUT_MAX_STRING_LENGTH, _L("Number of draw calls: \t\t %d"), mNumDrawCalls);
   mpDrawCallsText->SetText(string);
    
   CPUTDrawGUI();
}

// andrewmac:
void MySample::CreateTransformedModels(CPUTAssetSet **mpAssetSet, UINT numAssetSets)
{
   UINT numModels = 0;
   for(UINT assetId = 0; assetId < numAssetSets; assetId++)
   {
      for(UINT nodeId = 0; nodeId < mpAssetSet[assetId]->GetAssetCount(); nodeId++)
      {
         CPUTRenderNode* pRenderNode = NULL;
         CPUTResult result = mpAssetSet[assetId]->GetAssetByIndex(nodeId, &pRenderNode);
         ASSERT((CPUT_SUCCESS == result), _L ("Failed getting asset by index")); 
         if(pRenderNode->IsModel())
         {
               numModels++;        
         }
         pRenderNode->Release();
      }    
   }

   for(UINT assetId = 0; assetId < numAssetSets; assetId++)
   {
      for(UINT nodeId = 0; nodeId< mpAssetSet[assetId]->GetAssetCount(); nodeId++)
      {
         CPUTRenderNode* pRenderNode = NULL;
         CPUTResult result = mpAssetSet[assetId]->GetAssetByIndex(nodeId, &pRenderNode);
         ASSERT((CPUT_SUCCESS == result), _L ("Failed getting asset by index"));
         if(pRenderNode->IsModel())
         {
            CPUTModelDX11* model = (CPUTModelDX11*)pRenderNode;
            ASSERT((model != NULL), _L("model is NULL"));

            model = (CPUTModelDX11*)pRenderNode;
            TransformedModelScalar* occluder = mOcclusionTest->AddOccluder();
            CreateTransformedMeshes(occluder, model);
         }
         pRenderNode->Release();
      }
   }

   mOcclusionTest->RefreshOccluders();
}

void MySample::CreateTransformedMeshes(TransformedModelScalar* pTransformedModel, CPUTModelDX11 *pModel)
{
   float4x4* mWorldMatrix = pModel->GetWorldMatrix();
   pTransformedModel->SetWorldMatrix(mWorldMatrix);

   float3 os_center, os_half;
   pModel->GetBoundsObjectSpace(&os_center, &os_half);
   pTransformedModel->SetBoundsObjectSpace(os_center, os_half);

   float3 ws_center, ws_half;
   pModel->GetBoundsWorldSpace(&ws_center, &ws_half);
   pTransformedModel->SetBoundsWorldSpace(ws_center, ws_half);

   for(UINT i = 0; i < pModel->GetMeshCount(); i++)
   {
      CPUTMeshDX11* pMesh = (CPUTMeshDX11*)pModel->GetMesh(i);
      ASSERT((pMesh != NULL), _L("pMesh is NULL"));

      pTransformedModel->AddMesh(pMesh->GetDepthVertices(), pMesh->GetDepthVertexCount(), pMesh->GetDepthIndices(), pMesh->GetIndexCount(), pMesh->GetTriangleCount());
   }
}

//--------------------------------------------------------------------
// * Go through the asset set and determine the model count in it
// * Create data structures for all the models in the asset set
// * For each model create the axis aligned bounding box triangle 
//   vertex and index list
//--------------------------------------------------------------------
void MySample::CreateTransformedAABBoxes(CPUTAssetSet **pAssetSet, UINT numAssetSets)
{
   UINT numModels = 0;
   for(UINT assetId = 0; assetId < numAssetSets; assetId++)
   {
      for(UINT nodeId = 0; nodeId < pAssetSet[assetId]->GetAssetCount(); nodeId++)
      {
         CPUTRenderNode* pRenderNode = NULL;
         CPUTResult result = pAssetSet[assetId]->GetAssetByIndex(nodeId, &pRenderNode);
         ASSERT((CPUT_SUCCESS == result), _L ("Failed getting asset by index")); 
         if(pRenderNode->IsModel())
         {
            numModels++;        
         }
         pRenderNode->Release();
      }
   }
    
   for(UINT assetId = 0, modelId = 0; assetId < numAssetSets; assetId++)
   {
      for(UINT nodeId = 0; nodeId < pAssetSet[assetId]->GetAssetCount(); nodeId++)
      {
         CPUTRenderNode* pRenderNode = NULL;
         CPUTResult result = pAssetSet[assetId]->GetAssetByIndex(nodeId, &pRenderNode);
         ASSERT((CPUT_SUCCESS == result), _L ("Failed getting asset by index")); 
         if(pRenderNode->IsModel())
         {
            CPUTModelDX11 *pModel = (CPUTModelDX11*)pRenderNode;
            pModel = (CPUTModelDX11*)pRenderNode;
        
            TransformedAABBoxScalar* occludee = mOcclusionTest->AddOccludee();
            CreateAABBVertexIndexList(occludee, pModel);

            mpModels[modelId] = pModel;
            mNumModels++;
            pModel->AddRef();

            //mpTransformedAABBox[modelId].CreateAABBVertexIndexList(pModel);
            //mpNumTriangles[modelId] = 0;
            //for(int meshId = 0; meshId < pModel->GetMeshCount(); meshId++)
            //{
            //    mpNumTriangles[modelId] += pModel->GetMesh(meshId)->GetTriangleCount();
            //}
            modelId++;
         }
         pRenderNode->Release();
      }
   }
}

//--------------------------------------------------------------------------
// Get the bounding box center and half vector
// Create the vertex and index list for the triangles that make up the bounding box
//--------------------------------------------------------------------------
void MySample::CreateAABBVertexIndexList(TransformedAABBoxScalar* pTransformedBox, CPUTModelDX11 *pModel)
{
   pTransformedBox->SetWorldMatrix(pModel->GetWorldMatrix());

   float3 os_center, os_half;
   pModel->GetBoundsObjectSpace(&os_center, &os_half);
   pTransformedBox->SetBoundsObjectSpace(os_center, os_half);

   float3 ws_center, ws_half;
   pModel->GetBoundsWorldSpace(&ws_center, &ws_half);    
   pTransformedBox->SetBoundsWorldSpace(ws_center, ws_half);
}

char *gpDefaultShaderSource =  "\n\
// ********************************************************************************************************\n\
struct VS_INPUT\n\
{\n\
    float3 Pos      : POSITION; // Projected position\n\
    float3 Norm     : NORMAL;\n\
    float2 Uv       : TEXCOORD0;\n\
};\n\
struct PS_INPUT\n\
{\n\
    float4 Pos      : SV_POSITION;\n\
    float3 Norm     : NORMAL;\n\
    float2 Uv       : TEXCOORD0;\n\
    float4 LightUv  : TEXCOORD1;\n\
    float3 Position : TEXCOORD2; // Object space position \n\
};\n\
// ********************************************************************************************************\n\
    Texture2D    TEXTURE0 : register( t0 );\n\
    SamplerState SAMPLER0 : register( s0 );\n\
// ********************************************************************************************************\n\
cbuffer cbPerModelValues\n\
{\n\
    row_major float4x4 World : WORLD;\n\
    row_major float4x4 WorldViewProjection : WORLDVIEWPROJECTION;\n\
    row_major float4x4 InverseWorld : INVERSEWORLD;\n\
              float4   LightDirection;\n\
              float4   EyePosition;\n\
    row_major float4x4 LightWorldViewProjection;\n\
};\n\
// ********************************************************************************************************\n\
// TODO: Note: nothing sets these values yet\n\
cbuffer cbPerFrameValues\n\
{\n\
    row_major float4x4  View;\n\
    row_major float4x4  Projection;\n\
};\n\
// ********************************************************************************************************\n\
PS_INPUT VSMain( VS_INPUT input )\n\
{\n\
    PS_INPUT output = (PS_INPUT)0;\n\
    output.Pos      = mul( float4( input.Pos, 1.0f), WorldViewProjection );\n\
    output.Position = mul( float4( input.Pos, 1.0f), World ).xyz;\n\
    // TODO: transform the light into object space instead of the normal into world space\n\
    output.Norm = mul( input.Norm, (float3x3)World );\n\
    output.Uv   = float2(input.Uv.x, input.Uv.y);\n\
    output.LightUv   = mul( float4( input.Pos, 1.0f), LightWorldViewProjection );\n\
    return output;\n\
}\n\
// ********************************************************************************************************\n\
float4 PSMain( PS_INPUT input ) : SV_Target\n\
{\n\
    float3  lightUv = input.LightUv.xyz / input.LightUv.w;\n\
    lightUv.xy = lightUv.xy * 0.5f + 0.5f; // TODO: Move scale and offset to matrix.\n\
    lightUv.y  = 1.0f - lightUv.y;\n\
    float3 normal         = normalize(input.Norm);\n\
    float  nDotL          = saturate( dot( normal, -LightDirection ) );\n\
    float3 eyeDirection   = normalize(input.Position - EyePosition);\n\
    float3 reflection     = reflect( eyeDirection, normal );\n\
    float  rDotL          = saturate(dot( reflection, -LightDirection ));\n\
    float3 specular       = pow(rDotL, 16.0f);\n\
    float4 diffuseTexture = TEXTURE0.Sample( SAMPLER0, input.Uv );\n\
    float ambient = 0.05;\n\
    float3 result = (nDotL+ambient) * diffuseTexture + specular;\n\
    return float4( result, 1.0f );\n\
}\n\
\n\
// ********************************************************************************************************\n\
struct VS_INPUT_NO_TEX\n\
{\n\
    float3 Pos      : POSITION; // Projected position\n\
    float3 Norm     : NORMAL;\n\
};\n\
struct PS_INPUT_NO_TEX\n\
{\n\
    float4 Pos      : SV_POSITION;\n\
    float3 Norm     : NORMAL;\n\
    float4 LightUv  : TEXCOORD1;\n\
    float3 Position : TEXCOORD0; // Object space position \n\
};\n\
// ********************************************************************************************************\n\
PS_INPUT_NO_TEX VSMainNoTexture( VS_INPUT_NO_TEX input )\n\
{\n\
    PS_INPUT_NO_TEX output = (PS_INPUT_NO_TEX)0;\n\
    output.Pos      = mul( float4( input.Pos, 1.0f), WorldViewProjection );\n\
    output.Position = mul( float4( input.Pos, 1.0f), World ).xyz;\n\
    // TODO: transform the light into object space instead of the normal into world space\n\
    output.Norm = mul( input.Norm, (float3x3)World );\n\
    output.LightUv   = mul( float4( input.Pos, 1.0f), LightWorldViewProjection );\n\
    return output;\n\
}\n\
// ********************************************************************************************************\n\
float4 PSMainNoTexture( PS_INPUT_NO_TEX input ) : SV_Target\n\
{\n\
    float3 lightUv = input.LightUv.xyz / input.LightUv.w;\n\
    float2 uv = lightUv.xy * 0.5f + 0.5f;\n\
    float3 eyeDirection = normalize(input.Position - EyePosition.xyz);\n\
    float3 normal       = normalize(input.Norm);\n\
    float  nDotL = saturate( dot( normal, -normalize(LightDirection.xyz) ) );\n\
    float3 reflection   = reflect( eyeDirection, normal );\n\
    float  rDotL        = saturate(dot( reflection, -LightDirection.xyz ));\n\
    float  specular     = 0.2f * pow( rDotL, 4.0f );\n\
    return float4( (nDotL + specular).xxx, 1.0f);\n\
}\n\
";


