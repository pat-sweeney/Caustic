//**********************************************************************
// Copyright Patrick Sweeney 2019
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include "Rendering\Caustic\Caustic.h"
#include "IRenderMesh.h"
#include <d3d11.h>

export module Rendering.Caustic.RenderMesh;
import Base.Core.Core;
import Base.Math.BBox;
import Rendering.Caustic.Shader;
import Rendering.Caustic.IRenderMaterial;

//**********************************************************************
// File: RenderMesh.h
// Contains the declaration for the <CRenderMesh> class.
//**********************************************************************

export namespace Caustic
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
	//
	// Module:
	// {Link:import Rendering.Caustic.RenderMesh;{Rendering/Caustic/RenderMesh.ixx}}
	//**********************************************************************
    class CRenderSubMesh : public IRenderSubMesh, public CRefCount
	{
		std::string m_name;
        MeshData m_VB;
        MeshData m_NormalVB;
        CRefObj<IRenderMaterial> m_spFrontMaterial;
		CRefObj<IRenderMaterial> m_spBackMaterial;
        uint32 m_flags;

		void RenderSubMesh(IRenderer* pRenderer, IShader* pShader, IRenderMaterial* pMaterial,
			std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld);
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
		virtual void SetName(const char* pName) override { m_name = std::string(pName); }
		virtual std::string GetName() override { return m_name; }
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
		virtual void Render(IRenderer* pRenderer, IRenderCtx *pRenderCtx, IRenderMaterial* pFrontMaterialOverride, IRenderMaterial* pBackMaterialOverride, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld) override;
		virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, IShader* pShader, IRenderMaterial* pMaterial, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld) override;
////		virtual void SetShader(IShader *pShader) override { m_spShader = pShader; }
////		virtual CRefObj<IShader> GetShader() override { return m_spShader; }
	};

	//**********************************************************************
	// Class: CRenderMesh
	// Implements <IRenderMesh>
	//
	// Members:
	// m_subMeshes - List of submeshes that comprise this mesh
	// m_materials - list of materials used by the submeshes
	//
	// Module:
	// {Link:import Rendering.Caustic.RenderMesh;{Rendering/Caustic/RenderMesh.ixx}}
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
		virtual CRefObj<IRenderSubMesh> GetSubMesh(uint32 index) override;
		virtual void AddSubMesh(IRenderSubMesh *pSubMesh) override;
		virtual void GetBBox(Caustic::BBox3 *pBBox) override;
		virtual void SetMaterials(std::vector<CRefObj<IMaterialAttrib>> &materials) override;
		virtual CRefObj<IMaterialAttrib> GetMaterial(uint32 materialID) override;
		virtual void ComputeNormals() override {}
		virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, IRenderMaterial* pFrontMaterialOverride, IRenderMaterial* pBackMaterialOverride, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld) override;
		virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, IShader *pShader, IRenderMaterial*pMaterial, std::vector<CRefObj<ILight>>& lights, DirectX::XMMATRIX* pWorld) override;

		//**********************************************************************
		// ISerialize
		//**********************************************************************
		virtual void Load(IStream *pStream) override {}
		virtual void Store(IStream *pStream) override {}
	};
}
