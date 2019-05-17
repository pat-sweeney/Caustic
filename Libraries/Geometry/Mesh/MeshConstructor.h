//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#include "Geometry\Mesh\Mesh.h"
#include "Geometry\Mesh\IMesh.h"
#include "Geometry\Mesh\IMeshConstructor.h"
#include <vector>

namespace Caustic
{
    //**********************************************************************
    // CMeshConstructor defines a constructor object used to build
	// meshes (i.e. links vertices and edges together)
    //**********************************************************************
    class CMeshConstructor : public IMeshConstructor, public CRefCount
    {
        CRefObj<IMesh> m_spMesh;        // Current mesh being constructed
        CRefObj<CSubMesh> m_spSubMesh;  // Current submesh being constructed
        CFace *m_pCurFace;              // Current face being constructed
        CHalfEdge *m_pPrevEdge;         // Previous edge
        CGeomVertex *m_pPrevVertex;     // Previous vertex
    public:
        CMeshConstructor();
        
        void AddEdge(CGeomVertex *pTail, CGeomVertex *pHead);

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // IMeshConstructor
        //**********************************************************************
        virtual void MeshOpen() override;
        virtual void MeshClose(IMesh **ppMesh) override;
        virtual void SubMeshOpen() override;
        virtual void SubMeshClose(ISubMesh **ppSubMesh) override;
        virtual void FaceOpen() override;
        virtual void FaceClose() override;
        virtual void VertexAdd(Caustic::Vector3 &pos, Caustic::Vector3 &normal, Caustic::Vector2 &uv) override;
    };
}
