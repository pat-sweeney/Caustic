//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "IMesh.h"

namespace Caustic
{
    //**********************************************************************
    // \brief IMeshConstructor defines the public interface used to create new meshes
    //
    // IMeshConstructor works on building a mesh up face by face (via FaceOpen/FaceClose).
    //**********************************************************************
    struct IMeshConstructor : public IRefCount
    {
        virtual void MeshOpen() = 0;
        virtual void MeshClose(IMesh **ppMesh) = 0;
        virtual void SubMeshOpen() = 0;
        virtual void SubMeshClose(ISubMesh **ppSubMesh) = 0;
        virtual void FaceOpen() = 0;
        virtual void FaceClose() = 0;
        virtual void VertexAdd(Vector3 &pos, Vector3 &normal, Vector2 &uv) = 0;
    };

    CAUSTICAPI void CreateMeshConstructor(IMeshConstructor **ppMeshConstructor);
}
