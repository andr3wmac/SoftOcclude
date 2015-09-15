#pragma once
#ifndef __SOVIEW_H__
#define __SOVIEW_H__

#include "math/mathPlaceholder.h"

class SoView
{
public:

    SoView(const Matrix4 &view, const Matrix4 &proj, const Frustrum &);

    void SetFrustrumCulling(bool);
    void SetOcclusionCulling(bool);
    void SetReproject(bool);
};

#endif