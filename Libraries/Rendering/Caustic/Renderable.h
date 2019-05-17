//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Rendering\Caustic\Caustic.h"
#include "IRenderable.h"
#include "Base\Core\RefCount.h"
#include "Base\Core\CritSec.h"
#include "Base\Core\Event.h"
#include "Shader.h"
#include <memory>
#include <any>
#include <vector>
#include <atlbase.h>
#include <d3d12.h>
#include <dxgi1_6.h>

namespace Caustic
{
	//**********************************************************************
	// CRenderable defines a self contained renderable object
	//**********************************************************************
	class CRenderable : public IRenderable, public CRefCount
	{
	protected:
		CRefObj<IRenderMaterial> m_spFrontMaterial;
		CRefObj<IRenderMaterial> m_spBackMaterial;
		uint32 m_passes; // List of passes to render this object in
		DirectX::XMMATRIX m_xform; // Current transform to apply to object

		CVertexBuffer m_vertexBuffer;
		CVertexBuffer m_normalVB;

		void RenderMesh(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx, IRenderMaterial *pRenderMaterial, D3D12_CULL_MODE cullmode);
	public:
		CRenderable(CVertexBuffer *pVB, CVertexBuffer *pNormalVB, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat);
		CRenderable() {}
		friend class CRenderer;

		//**********************************************************************
		// IRefCount
		//**********************************************************************
		virtual uint32 AddRef() override { return CRefCount::AddRef(); }
		virtual uint32 Release() override { return CRefCount::Release(); }

		//**********************************************************************
		// IRenderable
		//**********************************************************************
		virtual Vector3 GetPos() override { return Vector3(DirectX::XMVectorGetX(m_xform.r[3]), DirectX::XMVectorGetY(m_xform.r[3]), DirectX::XMVectorGetZ(m_xform.r[3])); }
		virtual void Render(IRenderer *pRenderer, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx);
		virtual void SetTransform(DirectX::XMMATRIX &mat) override { m_xform = mat; }
		virtual DirectX::XMMATRIX &GetTransform() override { return m_xform; }
		virtual bool InPass(int pass) { return ((m_passes | (1 << Caustic::c_PassObjID)) & (1 << pass)) ? true : false; }
	};
}
