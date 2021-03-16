//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Base\Core\Core.h"
#include "IMesh.h"

namespace Caustic
{
    //**********************************************************************
    // Interface: IMeshConstructor
    // Defines the public interface used to create new meshes
    // IMeshConstructor works on building a mesh up face by face (via FaceOpen/FaceClose).
    //**********************************************************************
    struct IMeshConstructor : public IRefCount
    {
        virtual void MeshOpen() = 0;
        virtual CRefObj<IMesh> MeshClose() = 0;
        virtual void SubMeshOpen() = 0;
        virtual CRefObj<ISubMesh> SubMeshClose() = 0;
        virtual void FaceOpen() = 0;
        virtual void FaceClose() = 0;
        virtual void VertexAdd(Vector3 &pos, Vector3 &normal, Vector2 &uv) = 0;

        //**********************************************************************
        // Method: MeshFromDensityFunction
        // Creates a mesh from a density function using the marching cubes algorithm.
        // It is assumed that the density function occupies a unit cube.
        //
        // Parameters:
        // numBlocks - number of blocks to subdivide space in each direction
        // fn - density function
        //**********************************************************************
        virtual CRefObj<IMesh> MeshFromDensityFunction(int numBlocks, std::function<float(Vector3&)> fn) = 0;
    };

    CAUSTICAPI CRefObj<IMeshConstructor> CreateMeshConstructor();
}
