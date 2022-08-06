//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <memory>
#include <any>
#include <vector>
#include <atlbase.h>
#include <d3d11.h>

export module Rendering.Caustic.PointCloud;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.IRefCount;
import Base.Core.RefCount;
import Base.Core.Event;
import Base.Core.CritSec;
import Rendering.Caustic.Renderable;
import Rendering.Caustic.Shader;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.ISampler;
import Rendering.Caustic.IRenderable;
import Rendering.Caustic.IPointCloud;
import Rendering.Caustic.ITexture;

//**********************************************************************
// File: PointCloud.ixx
// Contains the declaration for the <CPointCloud> class.
//**********************************************************************

export namespace Caustic
{
	//**********************************************************************
	// Class: CPointCloud
	// Implements <IRenderable>
	//
	// Members:
	// m_spFrontMaterial - Material for rendering front faces
	// m_spBackMaterial - Material for rendering back faces
	// m_passes - Bit flags indicate which passes this object is rendered in
	// m_xform - Current transform to apply to object
	// m_vertexBuffer - vertex buffer
	// m_normalVB - vertex buffer with normal vectors
	//
	// Module:
	// {Link:import Rendering.Caustic.PointCloud;{Rendering/Caustic/PointCloud.ixx}}
	//**********************************************************************
	class CPointCloud : public IPointCloud, public CRefCount
	{
	protected:
		CRenderable m_renderable;
		CRefObj<ITexture> m_spDepthMap;
		CRefObj<ITexture> m_spRayMap;
		CRefObj<ITexture> m_spColorMap;
		CRefObj<IMaterialAttrib> m_spMaterialAttrib;
		CRefObj<IRenderMaterial> m_spRenderMaterial;
		CRefObj<IShader> m_spShader;
		CRefObj<IRenderSubMesh> m_spRenderMesh;
		CRefObj<ISampler> m_spColorSampler;
		CRefObj<ISampler> m_spDepthSampler;
		Matrix4x4 m_extrinsics;
		Matrix3x3 m_intrinsics;
	public:
		CPointCloud(IRenderer* pRenderer, uint32 width, uint32 height);
		CPointCloud() {}
		friend class CRenderer;

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// IRenderable
		//**********************************************************************
		virtual Vector3 GetPos() override { return m_renderable.GetPos(); }
		virtual void Render(IRenderer* pRenderer, std::vector<CRefObj<ILight>>& lights, IRenderCtx* pRenderCtx);
		virtual void SetTransform(DirectX::XMMATRIX& mat) override { m_renderable.SetTransform(mat); }
		virtual DirectX::XMMATRIX& GetTransform() override { return m_renderable.GetTransform(); }
		virtual bool InPass(int pass) { return m_renderable.InPass(pass); }
		virtual void SetSubMesh(IRenderSubMesh* pSubMesh) override { m_renderable.SetSubMesh(pSubMesh); }
		virtual CRefObj<IRenderSubMesh> GetSubMesh() override { return m_renderable.GetSubMesh(); }
		virtual void SetFrontMaterial(IRenderMaterial* pMaterial) override { m_renderable.SetFrontMaterial(pMaterial); }
		virtual CRefObj<IRenderMaterial> GetFrontMaterial() override { return m_renderable.GetFrontMaterial(); }
		virtual void SetBackMaterial(IRenderMaterial* pMaterial) override { m_renderable.SetBackMaterial(pMaterial); }
		virtual CRefObj<IRenderMaterial> GetBackMaterial() override { return m_renderable.GetBackMaterial(); }

		//**********************************************************************
		// IPointCloud
		//**********************************************************************
		virtual void SetDepthMap(IRenderer* pRenderer, IImage* pImage) override;
		virtual void SetRayMap(IRenderer* pRenderer, IImage* pImage) override;
		virtual void SetColorMap(IRenderer* pRenderer, IImage* pImage) override;
		virtual void SetColorExtrinsics(Matrix4x4& mat) override;
		virtual void SetColorIntrinsics(Matrix3x3& mat) override;
	};
}
