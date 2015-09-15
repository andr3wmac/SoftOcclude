#pragma once
#ifndef __SOFTOCCLUDE_H__
#define __SOFTOCCLUDE_H__

#include "SoIWorkerCache.h"

#include "core/soMesh.h"
#include "core/soView.h"
#include "core/soReference.h"
#include "core/math/mathPlaceholder.h"

class SoftOcclude
{
public:

    SoftOcclude(bool enableThreading); // False if ST or external threading lib

    SoGeometryID AddGeometry(SoGeometryType, const SoVertexBuffer &, const SoIndexBuffer &);
    void UpdateGeometry(SoGeometryID, const SoVertexBuffer &, const SoIndexBuffer &);

    // TypeID is an engine side enum that defines what type we are dealing with,
    // For example fineGrainOccluder or courseGrainOccluder
    // Later we can use this bitflag to include certain types in the tests.
    SoOccluderID AddOccluder(SoGeometryID, U32 typeID, Matrix4 worldTransform);
    void UpdateOccluder(SoOccluderID, SoGeometryID);
    void UpdateOccluder(SoOccluderID, Matrix4 worldTransform);

    // Not called occludee as we want to use the api for more stuff
    // Type ID can be used to for example differentiate lights from
    // objects
    SoObjectID AddSceneObject(U32 typeID, const AABB &);
    void UpdateSceneObject(SoObjectID, const AABB &);

    // Views define a look in the world and as such represent
    // Camera's and shadow casters etc.
    SoViewID AddView(const SoView &, U32 occluderTypes, U32 objectTypes );
    void UpdateView(SoViewID, const SoView &, U32 occluderTypes, U32 objectTypes );

    // Is object visible from a defined view 
    bool IsVisible(SoViewID, SoObjectID);

    SoObjectID FindNearest(SoObjectID, U32 targetTypes);
    void Find4Nearest(SoObjectID, U32 targetTypes, SoObjectID[4]);

    // Have all the updates take effect, for (optional) the requested types
    void RebuildSceneStatus(U32 types = 0xFFFFFFFF);

    // Call for an external threading lib
    void DefineUpdateWorkers(SoViewID, SoIWorkerCache *);

    // Call for lib to manage everything
    // Blocking!
    void DefineAndUpdateWorkers(SoViewID);
};


#endif