//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "Base/Math/BBox.h"
#include "IRenderMesh.h"
#include "Shader.h"
#include <d3d11.h>

//**********************************************************************
// File: RenderMesh.h
// Contains the declaration for the <CRenderMesh> class.
//**********************************************************************

namespace Caustic
{
	//**********************************************************************
    // Class: CRenderSubMesh
    // Implements <IRenderSubMesh>
    //
    // Members:
    // m_VB - Vertex/Index data for this submesh
    // m_NormalVB - Vertex/Index data for this submesh along with lines representing the normal vectors
    // m_spFrontMaterial - Material with which to render front facing polygons
    // m_spBackMaterial - Material with which to render back facing polygons
    // m_spShader - Shader to use for rendering
    // m_bbox - bounding box of submesh
    // m_flags - Mesh flags inherited from the ISubMesh
    //**********************************************************************
    class CRenderSubMesh : public IRenderSubMesh, public CRefCount
	{
        MeshData m_VB;
        MeshData m_NormalVB;
        CRefObj<IRenderMaterial> m_spFrontMaterial;
		CRefObj<IRenderMaterial> m_spBackMaterial;
		CRefObj<IShader> m_spShader;
        uint32 m_flags;
	public:
		CRenderSubMesh() :
			m_flags(0)
		{
		}

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// IRenderSubMesh
		//**********************************************************************
        virtual uint32 GetMeshFlags() override { return m_flags; }
        virtual void SetMeshFlags(uint32 flags) override { m_flags = flags; }
        virtual void SetFrontMaterial(IRenderMaterial *pFrontMaterial) override { m_spFrontMaterial = pFrontMaterial; }
		virtual CRefObj<IRenderMaterial> GetFrontMaterial() override { return m_spFrontMaterial; }
		virtual void SetBackMaterial(IRenderMaterial *pBackMaterial) override { m_spBackMaterial = pBackMaterial; }
		virtual CRefObj<IRenderMaterial> GetBackMaterial() override { return m_spBackMaterial; }
        virtual void SetMeshData(MeshData &meshData, bool hasNormals = false) override { ((hasNormals) ? m_NormalVB : m_VB) = meshData; }
        virtual const MeshData &GetMeshData(bool withNormals) override { return (withNormals) ? m_NormalVB : m_VB; }
        virtual void SetBBox(BBox3 *pBBox) override { m_VB.m_bbox = *pBBox; }
        virtual void GetBBox(BBox3 *pBBox) override { *pBBox = m_VB.m_bbox; };
		virtual void Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights) override;
		virtual void Render(IRenderer* pRenderer, IShader* pShader, IRenderMaterial* pMaterial, std::vector<CRefObj<IPointLight>>& lights) override;
		virtual void SetShader(IShader *pShader) override { m_spShader = pShader; }
		virtual CRefObj<IShader> GetShader() override { return m_spShader; }
	};

	//**********************************************************************
	class CRenderMesh : public IRenderMesh, public CRefCount
	{
		std::vector<CRefObj<IRenderSubMesh>> m_subMeshes;
		std::vector<CRefObj<IMaterialAttrib>> m_materials;
	public:
		CRenderMesh();

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// IRenderMesh
		//**********************************************************************
		virtual uint32 NumberSubMeshes() override { return (uint32)m_subMeshes.size(); };
		virtual void GetSubMesh(uint32 index, IRenderSubMesh **ppSubMesh) override;
		virtual void AddSubMesh(IRenderSubMesh *pSubMesh) override;
		virtual void GetBBox(Caustic::BBox3 *pBBox) override;
		virtual void SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials) override;
		virtual void GetMaterial(uint32 materialID, IMaterialAttrib **ppMaterial) override;
		virtual void ComputeNormals() override {}
		virtual void Render(IRenderer* pRenderer, std::vector<CRefObj<IPointLight>>& lights) override;
		virtual void Render(IRenderer* pRenderer, IShader *pShader, IRenderMaterial*pMaterial, std::vector<CRefObj<IPointLight>>& lights) override;

		//**********************************************************************
		// ISerialize
		//**********************************************************************
		virtual void Load(IStream *pStream) override {}
		virtual void Store(IStream *pStream) override {}
	};
}
