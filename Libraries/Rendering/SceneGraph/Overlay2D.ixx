//**********************************************************************
// Copyright Patrick Sweeney 2021-2023
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <functional>
#include <atlbase.h>
#include <string>
#include <d3d11.h>

export module Rendering.SceneGraph.SceneOverlay2DElem;
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.ISampler;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.Caustic.RendererFlags;
import Rendering.SceneGraph.ISceneOverlay2DElem;
import Parsers.JSon.IJSonParser;

export namespace Caustic
{
    //**********************************************************************
    // Class: CSceneOverlay2DElem
    // Defines a simple quad draw in screen coordinates
    //**********************************************************************
    class CSceneOverlay2DElem :
        public CSceneElem,
        public ISceneOverlay2DElem,
        public CRefCount
    {
    protected:
        BBox2 m_bounds;
        CRefObj<ITexture> m_spTexture;
        CComPtr<ID3D11Buffer> m_spQuadVB;
        CComPtr<ID3D11Buffer> m_spQuadIB;
        CRefObj<IShader> m_spShader;
        CRefObj<ISampler> m_spSampler;
        bool m_initialized;

        void CreateQuad(IRenderer* pRenderer);
    public:
        CSceneOverlay2DElem(IShader* pShader) :
            m_initialized(false)
        {
            m_spShader = pShader;
            m_passes = c_PassOpaque;
        }
        
        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override {}
        virtual void Store(IStream* pStream) override {}

        //**********************************************************************
        // IRefCount
        //**********************************************************************
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override
        {
//            CRefObj<IJSonObj> spShaderObj;
//            if (m_spShader)
//                spShaderObj = m_spShader->AsJson("Shader", pParser);
//            else
//                spShaderObj = pParser->CreateJSon("Shader", "<none>");
//            
//            CRefObj<IJSonObj> spSamplerObj;
//            if (m_spSampler)
//                spSamplerObj = m_spSampler->AsJson("Sampler", pParser);
//            else
//                spSamplerObj = pParser->CreateJSon("Sampler", "<none>");

            return pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "Overlay2D",
                pParser->CreateJSon("Type", "Overlay2D"),
                //spShaderObj,
                //spSamplerObj,
                pParser->CreateJSonMap("Bounds",
                    pParser->CreateJSonMap("MinPt",
                        pParser->CreateJSon("x", m_bounds.minPt.x),
                        pParser->CreateJSon("y", m_bounds.minPt.y),
                        nullptr),
                    pParser->CreateJSonMap("MaxPt",
                        pParser->CreateJSon("x", m_bounds.maxPt.x),
                        pParser->CreateJSon("y", m_bounds.maxPt.y),
                        nullptr),
                    nullptr),
                nullptr);
        }
        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::Overlay2D; }
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
        virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx) override;
        virtual void GetBBox(BBox3* pBBox) override {}
        virtual uint32_t GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32_t flags) override { m_Flags = flags; }
        virtual void SetInPass(uint32_t pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32_t GetInPass() override { return CSceneElem::GetInPass(); }

        //**********************************************************************
        // ISceneOverlay2DElem
        //**********************************************************************
        virtual void SetRect(BBox2& bbox) override { m_bounds = bbox; }
        virtual void SetTexture(ITexture* pTexture) override;
        virtual void SetShader(IShader* pShader) override
        {
            m_spShader = pShader;
        }
    };
}
