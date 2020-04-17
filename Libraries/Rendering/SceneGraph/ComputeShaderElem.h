//**********************************************************************
// Copyright Patrick Sweeney 2015-2020
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
#pragma once
#include "SceneGraph.h"
#include <d3d11.h>
#include <atlbase.h>

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

    public:
        //**********************************************************************
        // Constructor: CSceneComputeShaderElem
        // Default ctor
        //
        // Parameters:
        // pComputeShader - underlying compute shader
        //**********************************************************************
        CSceneComputeShaderElem(IShader* pComputeShader)
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
        virtual ESceneElemType GetType() { return ESceneElemType::ComputeShaderElem; }
        virtual std::wstring& Name() override { return CSceneElem::Name(); };
        virtual void Render(IRenderer* pRenderer, IRenderCtx* pRenderCtx, SceneCtx* pSceneCtx) override
        {
            std::vector<CRefObj<IPointLight>> lights;
            m_spComputeShader->BeginRender(pRenderer, nullptr, nullptr, lights, nullptr);
            m_spComputeShader->EndRender(pRenderer);
        }
        virtual void GetBBox(BBox3* pBBox) { CSceneElem::GetBBox(pBBox); }
        virtual uint32 GetFlags() override { return CSceneElem::GetFlags(); }
        virtual void SetFlags(uint32 flags) override { CSceneElem::SetFlags(flags); }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override { CT(E_NOTIMPL); }
        virtual void Store(IStream* pStream) override { CT(E_NOTIMPL); }

        //**********************************************************************
        // IComputeShaderElem
        //**********************************************************************
        virtual void SetInputBuffer(const wchar_t* pBufferName, uint8* pData, uint32 bufSize) override;
        virtual void SetOutputBuffer(const wchar_t* pBufferName, uint8* pData, uint32 bufSize) override;
    };
}
