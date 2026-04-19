//**********************************************************************
// Copyright Patrick Sweeney 2026
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <string>
#include <chrono>

export module Rendering.SceneGraph.SceneParticleSystemElem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Base.Math.BBox;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.RendererFlags;
import Rendering.Caustic.IParticleSystem;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneParticleSystemElem;

export namespace Caustic
{
    //**********************************************************************
    // Class: CSceneParticleSystemElem
    // Scene graph element that wraps a GPU particle system.
    // Handles both simulation (Update) and rendering during the
    // transparent pass.
    //**********************************************************************
    class CSceneParticleSystemElem :
        public CSceneElem,
        public ISceneParticleSystemElem,
        public CRefCount
    {
        CRefObj<IParticleSystem> m_spParticleSystem;
        std::chrono::high_resolution_clock::time_point m_lastTime;
        bool m_hasLastTime;
    public:
        CSceneParticleSystemElem(IParticleSystem* pParticleSystem)
            : m_hasLastTime(false)
        {
            m_spParticleSystem = pParticleSystem;
            m_Name = L"ParticleSystem";
        }

        // IRefCount
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        // ISceneElem
        virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override
        {
            auto spObj = pParser->CreateJSonMap((pPropertyName) ? pPropertyName : "ParticleSystem", nullptr);
            auto spBase = CSceneElem::AsJson(pPropertyName, pParser);
            spObj->AddElement(spBase);
            return spObj;
        }

        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::ParticleSystemElem; }
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

            // Only update and render during the transparent pass
            if (pRenderCtx->GetCurrentPass() == c_PassTransparent)
            {
                auto now = std::chrono::high_resolution_clock::now();
                if (m_hasLastTime)
                {
                    float dt = std::chrono::duration<float>(now - m_lastTime).count();
                    dt = (dt > 0.1f) ? 0.1f : dt;
                    m_spParticleSystem->Update(pRenderer, dt);
                }
                m_lastTime = now;
                m_hasLastTime = true;

                m_spParticleSystem->Render(pRenderer);
            }

            if (m_postrenderCallback)
                m_postrenderCallback(pRenderCtx->GetCurrentPass());
        }
        virtual void GetBBox(BBox3* pBBox) override { return; }
        virtual uint32_t GetFlags() override { return m_Flags; }
        virtual void SetFlags(uint32_t flags) override { m_Flags = flags; }
        virtual void SetInPass(uint32_t pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32_t GetInPass() override { return CSceneElem::GetInPass(); }

        // ISerialize
        virtual void Load(IStream* pStream) override { return; }
        virtual void Store(IStream* pStream) override { return; }

        // ISceneParticleSystemElem
        virtual CRefObj<IParticleSystem> GetParticleSystem() override { return m_spParticleSystem; }
    };
}
