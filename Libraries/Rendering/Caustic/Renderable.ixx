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
#include <DirectXMath.h>

export module Rendering.Caustic.Renderable;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.Event;
import Base.Core.CritSec;
import Rendering.Caustic.Shader;
import Rendering.Caustic.IRenderMaterial;
import Rendering.Caustic.IRenderable;
import Rendering.Caustic.RendererFlags;

//**********************************************************************
// File: Renderable.ixx
// Contains the declaration for the <CRenderable> class.
//**********************************************************************

export namespace Caustic
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
    //
    // Module:
    // {Link:import Rendering.Caustic.Renderable;{Rendering/Caustic/Renderable.ixx}}
    //**********************************************************************
    class CRenderable : public IRenderable, public CRefCount
    {
    protected:
        CRefObj<IRenderMaterial> m_spFrontMaterial;
        CRefObj<IRenderMaterial> m_spBackMaterial;
        uint32 m_passes;
        DirectX::XMMATRIX m_xform;
        CRefObj<IRenderSubMesh> m_spSubMesh;

        void RenderMesh(IRenderer* pRenderer, std::vector<CRefObj<ILight>> &lights, IRenderCtx *pRenderCtx, IRenderMaterial *pRenderMaterial, D3D11_CULL_MODE cullmode);
    public:
        CRenderable(IRenderSubMesh *pSubMesh, IRenderMaterial *pFrontMaterial, IRenderMaterial *pBackMaterial, DirectX::XMMATRIX &mat);
        CRenderable() : m_passes(1 << Caustic::c_PassOpaque)
        {
            m_xform = DirectX::XMMatrixIdentity();
        }
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
        virtual void Render(IRenderer* pRenderer, std::vector<CRefObj<ILight>> &lights, IRenderCtx *pRenderCtx);
        virtual void SetTransform(DirectX::XMMATRIX &mat) override { m_xform = mat; }
        virtual DirectX::XMMATRIX &GetTransform() override { return m_xform; }
        virtual bool InPass(int pass) { return ((m_passes | (1 << Caustic::c_PassObjID)) & (1 << pass)) ? true : false; }
        virtual void SetSubMesh(IRenderSubMesh* pSubMesh) override { m_spSubMesh = pSubMesh; }
        virtual CRefObj<IRenderSubMesh> GetSubMesh() override { return m_spSubMesh; }
        virtual void SetFrontMaterial(IRenderMaterial* pMaterial) override { m_spFrontMaterial = pMaterial; }
        virtual CRefObj<IRenderMaterial> GetFrontMaterial() override { return m_spFrontMaterial; }
        virtual void SetBackMaterial(IRenderMaterial* pMaterial) override { m_spBackMaterial = pMaterial; }
        virtual CRefObj<IRenderMaterial> GetBackMaterial() override { return m_spBackMaterial; }
    };
}
