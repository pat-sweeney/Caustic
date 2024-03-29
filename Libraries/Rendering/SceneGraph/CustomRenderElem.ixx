//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <string>

export module Rendering.SceneGraph.SceneCustomRenderElem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Geometry.Mesh.MeshFuncs;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.IRenderer;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneCustomRenderElem;

export namespace Caustic
{
    //**********************************************************************
    // Class: CSceneCustomRenderElem
    // Defines a mesh element in our scene graph
    //**********************************************************************
    class CSceneCustomRenderElem :
        public CSceneElem,
        public ISceneCustomRenderElem,
        public CRefCount
    {
        std::function<void(IRenderer * pRenderer, IRenderCtx * pRenderCtx, SceneCtx *pSceneCtx)> m_clientCallback;
    public:
        //**********************************************************************
        // Constructor: CSceneCustomRenderElem
        // Default constructor
        //**********************************************************************
        CSceneCustomRenderElem(std::function<void(IRenderer * pRenderer, IRenderCtx * pRenderCtx, SceneCtx * pSceneCtx)> clientCallback)
        {
            m_clientCallback = clientCallback;
        }

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override
        {
            auto spObj = pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "CustomRender", nullptr);
            auto spBase = CSceneElem::AsJson(pPropertyName, pParser);
            spObj->AddElement(spBase);
            return spObj;
        }

        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::Mesh; }
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
            m_clientCallback(pRenderer, pRenderCtx, pSceneCtx);
            if (m_postrenderCallback)
                m_postrenderCallback(pRenderCtx->GetCurrentPass());
        }
        virtual void GetBBox(BBox3* pBBox) override { return; }
        virtual uint32_t GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32_t flags) override { m_Flags = flags; }
        virtual void SetInPass(uint32_t pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32_t GetInPass() override { return CSceneElem::GetInPass(); }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override { return; }
        virtual void Store(IStream* pStream) override { return; }

        //**********************************************************************
        // ISceneCustomRenderElem
        //**********************************************************************
        virtual void SetCallback(std::function<void(IRenderer *pRenderer, IRenderCtx *pCtx, SceneCtx * pSceneCtx)> clientCallback) override
        {
            m_clientCallback = clientCallback;
        }
    };
};
