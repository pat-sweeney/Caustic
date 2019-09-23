//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// Licensed under the MIT license.
// See file LICENSE for details.
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
#include <d3d11.h>

//**********************************************************************
// File: Renderable.h
// Contains the declaration for the <CRenderable> class.
//**********************************************************************

namespace Caustic
{
	//**********************************************************************
    // Class: CRenderable
    // Implements <IRenderable>
    //
    // Members:
    // m_spFrontMaterial - Material for rendering front faces
    // m_spBackMaterial - Material for rendering back faces
    // m_passes - Bit flags indicate which passes this object is rendered in
    // m_xform - Current transform to apply to object
    // m_vertexBuffer - vertex buffer
    // m_normalVB - vertex buffer with normal vectors
    //**********************************************************************
	class CRenderable : public IRenderable, public CRefCount
	{
	protected:
		CRefObj<IRenderMaterial> m_spFrontMaterial;
		CRefObj<IRenderMaterial> m_spBackMaterial;
		uint32 m_passes;
		DirectX::XMMATRIX m_xform;
        CRefObj<IRenderSubMesh> m_spSubMesh;

		void RenderMesh(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx, IRenderMaterial *pRenderMaterial, D3D11_CULL_MODE cullmode);
	public:
        CRenderable(IRenderSubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat);
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
		virtual void Render(IGraphics *pGraphics, std::vector<CRefObj<IPointLight>> &lights, IRenderCtx *pRenderCtx);
		virtual void SetTransform(DirectX::XMMATRIX &mat) override { m_xform = mat; }
		virtual DirectX::XMMATRIX &GetTransform() override { return m_xform; }
		virtual bool InPass(int pass) { return ((m_passes | (1 << Caustic::c_PassObjID)) & (1 << pass)) ? true : false; }
	};
}
