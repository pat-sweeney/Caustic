//**********************************************************************
// Copyright Patrick Sweeney 2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>
#include <map>
#include <string>
#include <d3d11.h>

export module Geometry.Mesh.MeshFuncs;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.Vector;
import Geometry.Mesh.RenderTypes;
import Geometry.Mesh.IMesh;

export namespace Caustic
{
    CRefObj<IMesh> CreateSurfaceRevolution(std::vector<Vector3>& pts, uint32_t npts, uint32_t subdivisions, float maxAngle);
    CRefObj<IMesh> CreateEmptyMesh();
    CRefObj<ISubMesh> CreateSubMesh(std::vector<CGeomVertex>& verts,
        std::vector<int>& faces,
        uint32_t materialID);
    CRefObj<ISubMesh> CreateSubMesh(std::vector<Vector3>& vertPos,
        std::vector<Vector3>& vertNorms,
        std::vector<Vector2>& vertUVs,
        std::vector<int>& faces,
        EVertexFlags flags,
        uint32_t materialID);

    //**********************************************************************
    // Method: CreateEmptySubMesh
    // Creates a empty submesh.
    //**********************************************************************
    CRefObj<ISubMesh> CreateEmptySubMesh();
    CRefObj<IMesh> CreateCube();
    CRefObj<IMesh> CreateSphere(uint32_t subdivisions);
    CRefObj<IMesh> CreateTetrahedron();
    CRefObj<IMesh> CreateGrid(uint32_t subdivisions);
    CRefObj<IMesh> CreateGrid(uint32_t width, uint32_t height);
    CRefObj<IMesh> CreateGrid(uint32_t width, uint32_t height, float2* uvs);
    CRefObj<IMesh> CreateWarpedGrid(uint32_t width, uint32_t height, float2* pPositions);
};
