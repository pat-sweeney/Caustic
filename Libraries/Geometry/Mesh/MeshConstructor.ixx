//**********************************************************************
// Copyright Patrick Sweeney 2015-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <vector>

export module Geometry.Mesh.MeshConstructor;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Geometry.Mesh.Mesh;
import Geometry.Mesh.IMesh;
import Geometry.Mesh.IMeshConstructor;
import Geometry.Mesh.RenderTypes;

export namespace Caustic
{
    //**********************************************************************
    // Class: CMeshConstructor
    // Defines a constructor object used to build
    // meshes (i.e. links vertices and edges together)
    //**********************************************************************
    class CMeshConstructor : public IMeshConstructor, public CRefCount
    {
        CRefObj<IMesh> m_spMesh;        // Current mesh being constructed
        CRefObj<CSubMesh> m_spSubMesh;  // Current submesh being constructed
        CFace *m_pCurFace;              // Current face being constructed
        CHalfEdge *m_pPrevEdge;         // Previous edge
        CGeomVertex *m_pPrevVertex;     // Previous vertex

#ifdef CHECK_MARCHING_CUBES
        CRefObj<IMesh> BuildMarchingCubeCases();
#endif
    public:
        CMeshConstructor();
        
        void AddEdge(CGeomVertex *pTail, CGeomVertex *pHead);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IMeshConstructor
        //**********************************************************************
        virtual void MeshOpen() override;
        virtual CRefObj<IMesh> MeshClose() override;
        virtual void SubMeshOpen() override;
        virtual CRefObj<ISubMesh> SubMeshClose() override;
        virtual void FaceOpen() override;
        virtual void FaceClose() override;
        virtual void VertexAdd(Caustic::Vector3& pos, Caustic::Vector3& normal, Caustic::Vector2& uv) override;
        virtual void VertexAdd(Caustic::Vector3& pos, Caustic::Vector3& normal, Caustic::Vector2 uvs[4]) override;
        virtual CRefObj<IMesh> MeshFromDensityFunction(int numBlocks, std::function<float(Vector3&)> fn) override;
    };
}
