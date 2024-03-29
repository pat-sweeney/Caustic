//**********************************************************************
// Copyright Patrick Sweeney 2015-2024
// Licensed under the MIT license.
// See file LICENSE for details.
//**********************************************************************
module;
#include <d3d11.h>
#include <atlbase.h>
#include <string>
#include <cinttypes>

export module Rendering.SceneGraph.SceneComputeShaderElem;
import Base.Core.Core;
import Base.Core.Error;
import Base.Core.RefCount;
import Base.Core.IRefCount;
import Base.Core.ISerialize;
import Base.Math.Vector;
import Base.Math.Ray;
import Base.Math.Vector;
import Base.Math.BBox;
import Geometry.Mesh.IMaterialAttrib;
import Rendering.Caustic.IShader;
import Rendering.Caustic.IRenderer;
import Rendering.Caustic.IRenderCtx;
import Rendering.SceneGraph.SceneGraph;
import Rendering.SceneGraph.SceneElem;
import Rendering.SceneGraph.ISceneComputeShaderElem;
import Parsers.JSon.IJSonParser;

export namespace Caustic
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
        uint32_t m_xThreads, m_yThreads, m_zThreads;

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
        virtual uint32_t AddRef() override { return CRefCount::AddRef(); }
        virtual uint32_t Release() override { return CRefCount::Release(); }

        //**********************************************************************
        // ISceneElem
        //**********************************************************************
        virtual CRefObj<IJSonObj> AsJson(const char* pPropertyName, IJSonParser* pParser) override;
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
        virtual uint32_t GetFlags() override { return CSceneElem::GetFlags(); }
        virtual void SetFlags(uint32_t flags) override { CSceneElem::SetFlags(flags); }
        virtual void SetInPass(uint32_t pass) override { CSceneElem::SetInPass(pass); }
        virtual uint32_t GetInPass() override { return CSceneElem::GetInPass(); }

        //**********************************************************************
        // ISerialize
        //**********************************************************************
        virtual void Load(IStream* pStream) override { CT(E_NOTIMPL); }
        virtual void Store(IStream* pStream) override { CT(E_NOTIMPL); }

        //**********************************************************************
        // IComputeShaderElem
        //**********************************************************************
        virtual CRefObj<IShader> GetShader() override { return m_spComputeShader; }
        virtual void SetInputThreads(uint32_t width, uint32_t height, uint32_t depth = 1) override;
        virtual void SetShaderParam(const wchar_t* pParamName, uint32_t value) override;
        virtual void SetShaderParam(const wchar_t* pParamName, float value) override;
        virtual void SetBuffer(IRenderer* pRenderer, const wchar_t* pBufferName, uint8_t* pData, uint32_t bufSize, uint32_t elemSize) override;
        virtual void SetNumberThreads(int xThreads, int yThreads, int zThreads) override;
    };
}
