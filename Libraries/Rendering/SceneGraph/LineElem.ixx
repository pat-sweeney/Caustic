//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <string>

export module Rendering.SceneGraph.SceneLineElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Math.BBox;
import Base.Math.Vector;
import Geometry.Mesh.IMaterialAttrib;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneLineElem;

export namespace Caustic
{
    //**********************************************************************
    // Class: CSceneLineElem
    // Defines a line element in our scene graph
    //**********************************************************************
    class CSceneLineElem :
        public CSceneElem,
        public ISceneLineElem,
        public CRefCount
    {
        Vector3 m_p0;
        Vector3 m_p1;
    public:
        //**********************************************************************
        // Constructor: CSceneLineElem
        // Default constructor
        //**********************************************************************
        CSceneLineElem(Vector3& p0, Vector3& p1)
        {
            m_p0 = p0;
            m_p1 = p1;
        }

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override
        {
            auto spObj = pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "Line", nullptr);
            auto spBase = CSceneElem::AsJson("Base", pParser);
            spObj->AddElement(spBase);
            spObj->AddElement(
                pParser->CreateJSonMap("P0",
                    pParser->CreateJSon("x", m_p0.x),
                    pParser->CreateJSon("y", m_p0.y),
                    pParser->CreateJSon("z", m_p0.z),
                    nullptr));
            spObj->AddElement(
                pParser->CreateJSonMap("P1",
                    pParser->CreateJSon("x", m_p1.x),
                    pParser->CreateJSon("y", m_p1.y),
                    pParser->CreateJSon("z", m_p1.z),
                    nullptr));
            return spObj;
        }
        
        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::LineElem; }
        virtual std::wstring GetName() override { return CSceneElem::GetName(); }
        virtual void SetName(const wchar_t* name) override { return CSceneElem::SetName(name); }
        virtual void SetPreRenderCallback(std::function<bool(int pass)> prerenderCallback) override
        {
            CSceneElem::SetPreRenderCallback(prerenderCallback);
        }
        virtual void SetPostRenderCallback(std::function<void(int pass)> postrenderCallback) override
        {
            CSceneElem::SetPostRenderCallback(postrenderCallback);
        }
        virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx) override
        {
            if (!(m_passes & pRenderCtx->GetCurrentPass()))
                return;
            if (m_prerenderCallback)
                if (!m_prerenderCallback(pRenderCtx->GetCurrentPass()))
                    return;
            Vector4 vc;
            vc.x = 1.0f;
            vc.y = 1.0f;
            vc.z = 1.0f;
            vc.w = 1.0f;
            pRenderer->DrawLine(m_p0, m_p1, vc);
            if (m_postrenderCallback)
                m_postrenderCallback(pRenderCtx->GetCurrentPass());
        }
        virtual void GetBBox(BBox3* pBBox) override { return; }
        virtual uint32 GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32 flags) override { m_Flags = flags; }
        virtual void SetInPass(uint32 pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32 GetInPass() override { return CSceneElem::GetInPass(); }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override { return; }
        virtual void Store(IStream* pStream) override { return; }

        //**********************************************************************
        // ISceneLineElem
        //**********************************************************************
        virtual void SetPosition(Vector3& p0, Vector3& p1) override
        {
            m_p0 = p0;
            m_p1 = p1;
        }

        virtual void GetPosition(Vector3* p0, Vector3* p1) override
        {
            *p0 = m_p0;
            *p1 = m_p1;
        }
    };
};
