#pragma once
#ifndef __SOMESH_H__
#define __SOMESH_H__

#include "core/common/types.h"

enum class SoGeometryType
{
    TriangleList = 1,
    TriangleStrip = 2,
    QuadList = 3
};

class SoVertex
{
public:

    SoVertex(F32 x, F32 y, F32 z);
};

class SoVertexBuffer
{
public:

    SoVertexBuffer();

    void AddVertex(const SoVertex &);
};

class SoIndexBuffer
{
public:

    SoIndexBuffer();

    void AddIndex(U32);
};

#endif