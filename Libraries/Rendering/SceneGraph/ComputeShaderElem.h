//**********************************************************************
// Copyright Patrick Sweeney 2015-2021
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include <d3d11.h>
#include <atlbase.h>
#include "SceneGraph.h"
import Base.Core.Core;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Rendering.Caustic.IRenderCtx;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IShader;

namespace Caustic
{
    //**********************************************************************
    // Class: CSceneComputeShaderElem
    // Defines a compute shader. Compute shaders are run at the very beginning
    // of scene graph traversal (i.e. before rendering).
    //**********************************************************************
    class CSceneComputeShaderElem :
        public CSceneElem,
        public ISceneComputeShaderElem,
        public CRefCount
    {
        CRefObj<IShader> m_spComputeShader;
        uint32 m_xThreads, m_yThreads, m_zThreads;

    public:
        //**********************************************************************
        // Constructor: CSceneComputeShaderElem
        // Default ctor
        //
        // Parameters:
        // pComputeShader - underlying compute shader
        //**********************************************************************
        CSceneComputeShaderElem(IShader* pComputeShader) :
            m_xThreads(1), m_yThreads(1), m_zThreads(1)
        {
            m_spComputeShader = pComputeShader;
        }

        //**********************************************************************
        // IUnknown
        //**********************************************************************
        virtual uint32 AddRef() override { return CRefCount::AddRef(); }
        virtual uint32 Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual bool RayIntersect(Ray3& ray, RayIntersect3* pIntersection, IMaterialAttrib** pMaterial) override { return false; }
        virtual ESceneElemType GetType() override { return ESceneElemType::ComputeShaderElem; }
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
            m_spComputeShader->Dispatch(pRenderer, m_xThreads, m_yThreads, m_zThreads);
            if (m_postrenderCallback)
                m_postrenderCallback(pRenderCtx->GetCurrentPass());
        }
        virtual void GetBBox(BBox3* pBBox) { CSceneElem::GetBBox(pBBox); }
        virtual uint32 GetFlags() override { return CSceneElem::GetFlags(); }
        virtual void SetFlags(uint32 flags) override { CSceneElem::SetFlags(flags); }
        virtual void SetInPass(uint32 pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32 GetInPass() override { return CSceneElem::GetInPass(); }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override { CT(E_NOTIMPL); }
        virtual void Store(IStream* pStream) override { CT(E_NOTIMPL); }

        //**********************************************************************
        // IComputeShaderElem
        //**********************************************************************
        virtual CRefObj<IShader> GetShader() override { return m_spComputeShader; }
        virtual void SetInputThreads(uint32 width, uint32 height, uint32 depth = 1) override;
        virtual void SetShaderParam(const wchar_t* pParamName, uint32 value) override;
        virtual void SetShaderParam(const wchar_t* pParamName, float value) override;
        virtual void SetBuffer(IRenderer* pRenderer, const wchar_t* pBufferName, uint8* pData, uint32 bufSize, uint32 elemSize) override;
        virtual void SetNumberThreads(int xThreads, int yThreads, int zThreads) override;
    };
}
