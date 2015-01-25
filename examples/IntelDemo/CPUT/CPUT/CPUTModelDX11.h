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
#ifndef __CPUTMODELDX11_H__
#define __CPUTMODELDX11_H__

#include "CPUTModel.h"
#include "CPUT_DX11.h"

class CPUTMeshDX11;
class CPUTRenderParametersDX;
class CPUTMaterialDX11;

//--------------------------------------------------------------------------------------
struct CPUTModelConstantBuffer
{
    XMMATRIX  World;
    XMMATRIX  WorldViewProjection;
    XMMATRIX  InverseWorld;
    XMVECTOR  LightDirection;
    XMVECTOR  EyePosition;
    XMMATRIX  LightWorldViewProjection;
    XMMATRIX  ViewProjection;
    XMVECTOR  BoundingBoxCenterWorldSpace;
    XMVECTOR  BoundingBoxHalfWorldSpace;
    XMVECTOR  BoundingBoxCenterObjectSpace;
    XMVECTOR  BoundingBoxHalfObjectSpace;
};

//--------------------------------------------------------------------------------------
class CPUTModelDX11 : public CPUTModel
{
    friend class CPUTMaterialDX11;
protected:
    static ID3D11Buffer *mpModelConstantBuffer;

    // Destructor is not public.  Must release instead of delete.
    ~CPUTModelDX11()
    {
        SAFE_RELEASE(mpModelConstantBuffer); // TODO: This is a static.  Better to do somewhere else?
    }

    static UINT                  mFCullCount;

public:
    CPUTModelDX11(){}

    void* operator new(size_t i)
    {
        return _mm_malloc(i, 16);
    }

    void operator delete(void* p)
    {
        _mm_free(p);
    }

    static void CreateModelConstantBuffer();

    CPUTMeshDX11 *GetMesh(const UINT index) const;
    CPUTResult    LoadModel(CPUTConfigBlock *pBlock, int *pParentID, CPUTModel *pMasterModel=NULL);
    void          UpdateShaderConstants(CPUTRenderParameters &renderParams);
    void          Render(CPUTRenderParameters &renderParams);
    void          RenderShadow(CPUTRenderParameters &renderParams);
    void          SetMaterial(UINT ii, CPUTMaterial *pMaterial);
    void          DrawBoundingBox(CPUTRenderParameters &renderParams);
    void          CreateBoundingBoxMesh();

    static void ResetFCullCount(){mFCullCount = 0;}
    static UINT GetFCullCount(){return mFCullCount;}
};


#endif // __CPUTMODELDX11_H__